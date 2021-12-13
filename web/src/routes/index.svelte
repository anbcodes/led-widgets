<script lang="ts">
	import { onMount } from 'svelte';

	const newItem = {
		name: '',
		color: '',
		done: false,
		len: 0
	};

	let ws = null as WebSocket | null;
	let todo = [] as {
		name: string;
		color: string;
		done: boolean;
		len: number;
	}[];

	const send = (command: number, data: number[]) => {
		const cmdPart1 = command % 256;
		const cmdPart2 = (command / 256) % 256;

		const lenPart1 = data.length % 256;
		const lenPart2 = (data.length / 256) % 256;
		const lenPart3 = (data.length / 256 / 256) % 256;
		const lenPart4 = (data.length / 256 / 256 / 256) % 256;

		const sending = [cmdPart1, cmdPart2, lenPart1, lenPart2, lenPart3, lenPart4, ...data].map((v) =>
			Math.floor(v)
		);

		console.log(sending.map((v) => `\\x${v.toString(16).padStart(2, '0')}`).join(''));

		ws?.send(new Uint8Array(sending));
	};

	const word = (num: number) => [Math.floor(num % 256), Math.floor((num / 256) % 256)];

	type Color = { r: number; g: number; b: number; a: number };

	const sendLine = (
		index: number,
		offset: number,
		length: number,
		color: Color,
		animate = true
	) => {
		send(2 + +!animate, [
			...word(length),
			...word(offset),
			index,
			color.r,
			color.g,
			color.b,
			color.a
		]);
	};

	const addRedLine = () =>
		sendLine(0, 160, 14, {
			r: 255,
			g: 0,
			b: 0,
			a: 255
		});

	const addItem = () => {
		todo.push({
			name: newItem.name,
			color: newItem.color,
			len: newItem.len,
			done: false
		});

		todo = todo;

		saveTodo();

		newItem.name = '';
		newItem.color = '#000000';
		newItem.len = 0;
	};

	let mounted = false;
	let offsetTotal = 100;

	onMount(() => {
		todo = JSON.parse(localStorage.getItem('todo')) || [];
		ws = new WebSocket('ws://localhost:10101');
		ws.addEventListener('open', () => {
			todo.forEach((item, i) => {
				if (item.done) {
					sendLine(i, offsetTotal, item.len, fromHexColor(item.color), false);
					offsetTotal += item.len;
				}
			});
		});
		ws.addEventListener('message', async (ev) => {
			console.log(ev, await ev.data.text());
		});
		mounted = true;
	});

	const fromHexColor = (s: string) => {
		let matches = s.match(
			/#([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])/
		);

		console.log(matches);

		return {
			r: parseInt(matches[1], 16),
			g: parseInt(matches[2], 16),
			b: parseInt(matches[3], 16),
			a: 255
		};
	};

	const saveTodo = () => {
		console.log(todo);
		localStorage.setItem('todo', JSON.stringify(todo));
	};

	console.log(fromHexColor('#ffffff'));
</script>

<h1>Welcome to led todo list</h1>

<div>
	{#each todo as item, i}
		<div class="flex">
			<div
				class="m-2 w-5 h-5 duration-500 transition-colors bg-gray-800 border-gray-500 border"
				style="background-color: {!item.done ? `rgb(255, 255, 255)` : 'rgb(31 41 55)'}"
				on:click={() => {
					item.done = !item.done;
					if (item.done) {
						sendLine(i, offsetTotal, item.len, fromHexColor(item.color));
						offsetTotal += item.len;
					} else {
						sendLine(i, 0, 0, { r: 0, g: 0, b: 0, a: 0 }, false);
						offsetTotal -= item.len;
					}
					saveTodo();
				}}
			/>
			<div><input type="text" bind:value={item.name} on:change={saveTodo} /></div>

			<div class="m-2">
				<input type="color" bind:value={item.color} on:change={saveTodo} />
			</div>
			<div class="m-2">
				<input type="number" bind:value={item.len} on:change={saveTodo} />
			</div>
		</div>
	{/each}
</div>

<div>Add item</div>
<div>
	<div>Name:</div>
	<div><input type="text" bind:value={newItem.name} /></div>
	<div>Color:</div>
	<div><input type="color" bind:value={newItem.color} /></div>
	<div>Length:</div>
	<div><input type="number" bind:value={newItem.len} /></div>
	<button on:click={addItem}>Add!</button>
</div>

<button on:click={addRedLine}> send light </button>
