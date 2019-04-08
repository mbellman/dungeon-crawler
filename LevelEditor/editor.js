(function(){
	var blockMap = {
		0: { name: 'Empty', file: null },
		1: { name: 'Solid 1', file: 'solid_1.png' },
		2: { name: 'Solid 2', file: 'solid_2.png' },
		3: { name: 'Column B', file: 'column_bottom.png' },
		4: { name: 'Column M', file: 'column_middle.png' },
		5: { name: 'Column T', file: 'column_top.png' },
		6: { name: 'Staircase F', file: 'stairface_forward.png' },
		7: { name: 'Staircase B', file: 'staircase_backward.png' },
		8: { name: 'Staircase L', file: 'staircase_left.png' },
		9: { name: 'Staircase R', file: 'staircase_right.png' },
		10: { name: 'Bridge', file: 'bridge.png' }
	};

	var entities = [
		{ name: 'Torch', file: 'torch.png' },
		{ name: 'Chest', file: 'chest.png' }
	];

	var Direction = {
		FORWARD: 0,
		LEFT: 1,
		BACKWARD: 2,
		RIGHT: 3
	};

	var appState = {
		layerSize: {
			width: 25,
			height: 25
		},
		spawn: {
			layer: 1,
			x: 1,
			z: 1,
			direction: Direction.FORWARD
		},
		ambientLight: {
			color: { R: 0, G: 0, B: 0 },
			vector: { x: 0.0, y: -1.0, z: 0.0 },
			power: 0.0
		},
		visibility: 5000,
		brightness: 1.0,
		torches: [],
		chests: [],
		layers: [],
		currentLayer: 0,
		currentPlacementType: 'block',
		currentBlockType: 0,
		currentEntityType: 0,
	};

	function clamp(value, low, high) {
		return value > high ? high : value < low ? low : value;
	}

	function $(selector) {
		return document.querySelector(selector);
	}

	function $$(selector) {
		return Array.prototype.slice.call(document.querySelectorAll(selector), 0);
	}

	function unfocusButtonGroup(className) {
		$$('.' + className).forEach(function(button){
			button.setAttribute('class', 'ui-button ' + className);
		});
	}

	function unfocusPlacementButtons() {
		unfocusButtonGroup('block-button');
		unfocusButtonGroup('entity-button');
	}

	function createBlockButtonClickHandler(blockType) {
		return function(e) {
			e.currentTarget.setAttribute('class', 'ui-button block-button selected');

			appState.currentBlockType = blockType;
			appState.currentPlacementType = 'block';
		};
	}

	function createBlockButton(blockType) {
		var button = document.createElement('input');
		var name = blockMap[blockType].name;

		button.setAttribute('type', 'button');
		button.value = name;
		button.setAttribute('class', 'ui-button block-button');
		button.addEventListener('mousedown', unfocusPlacementButtons);
		button.addEventListener('click', createBlockButtonClickHandler(blockType));

		return button;
	}

	function createBlockButtons() {
		var $container = $('#block-buttons');
		var totalBlockTypes = Object.keys(blockMap).length;

		for (var i = 0; i < totalBlockTypes; i++) {
			$container.appendChild(createBlockButton(i));
		}
	}

	function createEntityButtonClickHandler(entityType) {
		return function(e) {
			e.currentTarget.setAttribute('class', 'ui-button entity-button selected');

			appState.currentEntity = entityType;
			appState.currentPlacementType = 'entity';
		};
	}

	function createEntityButton(entityType) {
		var button = document.createElement('input');
		var name = entities[entityType].name;

		button.setAttribute('type', 'button');
		button.value = name;
		button.setAttribute('class', 'ui-button entity-button');
		button.addEventListener('mousedown', unfocusPlacementButtons);
		button.addEventListener('click', createEntityButtonClickHandler(entityType));

		return button;
	}

	function createEntityButtons() {
		var $container = $('#entity-buttons');

		for (var i = 0; i < entities.length; i++) {
			$container.appendChild(createEntityButton(i));
		}
	}

	function addLayer() {
		var layer = [];

		for (var z = 0; z < appState.layerSize.height; z++) {
			var row = [];

			layer.push(row);

			for (var x = 0; x < appState.layerSize.width; x++) {
				row.push(Math.random() < 0.5 ? 1 : 0);
			}
		}

		appState.layers.push(layer);
	}

	function renderBlockType(context, blockType, rect) {
		var color = blockType === 0 ? '#000' : '#b61';

		context.beginPath();
		context.rect(rect.x, rect.y, rect.width, rect.height);
		context.fillStyle = color;
		context.fill();
	}

	function updateLayout() {
		$('#layout-current-layer').value = appState.currentLayer + '/' + (appState.layers.length - 1);

		var $canvas = $('#layout-canvas');
		var canvas = $canvas.getContext('2d');
		var blockWidth = $canvas.clientWidth / appState.layerSize.width;
		var blockHeight = $canvas.clientHeight / appState.layerSize.height;
		var currentLayer = appState.layers[appState.currentLayer];

		$canvas.setAttribute('width', $canvas.clientWidth);
		$canvas.setAttribute('height', $canvas.clientHeight);
		canvas.clearRect(0, 0, $canvas.clientWidth, $canvas.clientHeight);

		for (var z = 0; z < currentLayer.length; z++) {
			var row = currentLayer[z];

			for (var x = 0; x < row.length; x++) {
				var blockType = row[x];

				var rect = {
					x: blockWidth * x,
					y: blockHeight * z,
					width: blockWidth,
					height: blockHeight
				};

				renderBlockType(canvas, blockType, rect);
			}
		}
	}

	function updateOutput() {
		var output = '';

		function newline() {
			output += '\n';
		}

		function write(string) {
			output += string;
		}

		write(`LS ${appState.layerSize.width}, ${appState.layerSize.height}`);
		newline();
		write(`SP ${appState.spawn.layer}, ${appState.spawn.x}, ${appState.spawn.z}, ${appState.spawn.direction}`);
		newline();
		write(`AL ${appState.ambientLight.color.R}, ${appState.ambientLight.color.G}, ${appState.ambientLight.color.B}, `);
		write(`${appState.ambientLight.vector.x}, ${appState.ambientLight.vector.y}, ${appState.ambientLight.vector.z}, `);
		write(`${appState.ambientLight.power}`);
		newline();
		write(`V ${appState.visibility}`);
		newline();
		write(`B ${appState.brightness}`);
		newline();
		newline();

		for (var i = 0; i < appState.torches.length; i++) {
			var torch = appState.torches[i];

			write(`T ${torch.layer}, ${torch.x}, ${torch.z}, ${torch.direction}`)
			newline();
		}

		newline();

		for (var i = 0; i < appState.chests.length; i++) {
			var chest = appState.chests[i];

			write(`T ${chest.layer}, ${chest.x}, ${chest.z}, ${chest.direction}, ${chest.itemType}`)
			newline();
		}

		newline();

		for (var i = 0; i < appState.layers.length; i++) {
			var layer = appState.layers[i];

			write('L');
			newline();

			for (var z = 0; z < layer.length; z++) {
				var row = layer[z];

				for (var x = 0; x < row.length; x++) {
					write(`${row[x]}`);

					if (x < row.length - 1) {
						write(', ');
					}
				}

				newline();
			}

			if (i < appState.layers.length - 1) {
				newline();
			}
		}

		$('#output-textarea').value = output;
	}

	function syncSettingsInputs() {
		$('#settings-width').value = appState.layerSize.width;
		$('#settings-height').value = appState.layerSize.height;

		$('#settings-spawn-layer').value = appState.spawn.layer;
		$('#settings-spawn-x').value = appState.spawn.x;
		$('#settings-spawn-z').value = appState.spawn.z;
		$('#settings-spawn-d').value = appState.spawn.direction;

		$('#settings-al-r').value = appState.ambientLight.color.R;
		$('#settings-al-g').value = appState.ambientLight.color.G;
		$('#settings-al-b').value = appState.ambientLight.color.B;
		$('#settings-al-x').value = appState.ambientLight.vector.x;
		$('#settings-al-y').value = appState.ambientLight.vector.y;
		$('#settings-al-z').value = appState.ambientLight.vector.z;
		$('#settings-al-power').value = appState.ambientLight.power;

		$('#settings-visibility').value = appState.visibility;
		$('#settings-brightness').value = appState.brightness;
	}

	function upLayer() {
		appState.currentLayer = Math.min(appState.currentLayer + 1, appState.layers.length - 1);

		updateLayout();
	}

	function downLayer() {
		appState.currentLayer = Math.max(appState.currentLayer - 1, 0);

		updateLayout();
	}

	function bindEvents() {
		$('#layout-layer-up').addEventListener('click', upLayer);
		$('#layout-layer-down').addEventListener('click', downLayer);

		$('#layout-layer-add').addEventListener('click', function(){
			addLayer();
			updateLayout();
			updateOutput();
		});

		$('#output-textarea').addEventListener('click', function(){
			$('#output-textarea').setSelectionRange(0, $('#output-textarea').value.length);

			setTimeout(function(){
				$('#output-textarea').scrollTop = 0;
			}, 20);
		});
	}

	function initializeEditor() {
		for (var i = 0; i < 2; i++) {
			addLayer();
		}

		syncSettingsInputs();
		updateLayout();
		createBlockButtons();
		createEntityButtons();
		bindEvents();

		updateOutput();
	}

	window.initializeEditor = initializeEditor;
})();
