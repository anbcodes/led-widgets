<script lang="ts">
	import { onMount } from 'svelte';

	onMount(() => {
		const ws = new WebSocket('ws://localhost:10101');
		ws.addEventListener('open', () => {
			let data = new Uint8Array([
				0x02, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0a, 0x00, 0xa0, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff
			]);
			ws.send(data);
		});
		ws.addEventListener('message', (ev) => {
			console.log(ev);
		});
	});
</script>

<h1>Welcome to led todo list</h1>
