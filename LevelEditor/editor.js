(function(){
	var blockAssets = [];
	var entityAssets = [];

	var blockMap = {
		0: { name: 'Empty', file: null },
		1: { name: 'Solid 1', file: 'solid_1.png' },
		2: { name: 'Solid 2', file: 'solid_2.png' },
		3: { name: 'Column B', file: 'column_bottom.png' },
		4: { name: 'Column M', file: 'column_middle.png' },
		5: { name: 'Column T', file: 'column_top.png' },
		6: { name: 'Staircase F', file: 'staircase_forward.png' },
		7: { name: 'Staircase B', file: 'staircase_backward.png' },
		8: { name: 'Staircase L', file: 'staircase_left.png' },
		9: { name: 'Staircase R', file: 'staircase_right.png' },
		10: { name: 'Bridge', file: 'bridge.png' }
	};

	var entities = [
		{ name: 'Torch', file: 'torch.png' },
		{ name: 'Chest', file: 'chest.png' }
	];

	var directions = [
		'Forward',
		'Left',
		'Backward',
		'Right'
	];

	var BlockTypes = {
		EMPTY: 0,
		SOLID_1: 1,
		SOLID_2: 2,
		COLUMN_B: 3,
		COLUMN_M: 4,
		COLUMN_T: 5,
		STAIRCASE_F: 6,
		STAIRCASE_B: 7,
		STAIRCASE_L: 8,
		STAIRCASE_R: 9,
		BRIDGE: 10
	};

	var EntityTypes = {
		TORCH: 0,
		CHEST: 1
	};

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
		currentEntityOptions: null
	};

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

			clearEntityOptions();
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

	function clearEntityOptions() {
		$('#entity-options').innerHTML = '';

		appState.currentEntityOptions = {};
	}

	function showEntityDirectionOptions() {
		for (var i = 0; i < 4; i++) {
			var $directionButton = document.createElement('input');

			$directionButton.setAttribute('type', 'button');
			$directionButton.setAttribute('class', 'ui-button direction-button');
			$directionButton.setAttribute('data-index', i);
			$directionButton.value = directions[i];

			$directionButton.addEventListener('click', function(e){
				unfocusButtonGroup('direction-button');

				e.currentTarget.setAttribute('class', 'ui-button direction-button selected');

				appState.currentEntityOptions.direction = parseInt(e.currentTarget.getAttribute('data-index'));
			});

			$('#entity-options').appendChild($directionButton);
		}

		$$('.direction-button')[0].click();
	}

	function addEntityOptionField(label, defaultValue, changeHandler) {
		var $container = document.createElement('div');
		var $label = document.createElement('label');
		var $input = document.createElement('input');

		$label.innerHTML = `${label}: `;

		$input.setAttribute('size', '1');
		$input.value = defaultValue;
		$input.addEventListener('change', function() {
			changeHandler($input.value);
		});

		changeHandler(defaultValue);

		$container.appendChild($label);
		$container.appendChild($input);

		$('#entity-options').appendChild($container);
	}

	function showChestEntityOptions() {
		showEntityDirectionOptions();

		addEntityOptionField('Item', 0, function(value) {
			appState.currentEntityOptions.itemType = parseInt(value);
		});
	}

	function showEntityOptions(entityType) {
		clearEntityOptions();

		switch (entityType) {
			case EntityTypes.TORCH:
				showEntityDirectionOptions();
				break;
			case EntityTypes.CHEST:
				showChestEntityOptions();
				break;
		}
	}

	function createEntityButtonClickHandler(entityType) {
		return function(e) {
			e.currentTarget.setAttribute('class', 'ui-button entity-button selected');

			appState.currentEntityType = entityType;
			appState.currentPlacementType = 'entity';

			showEntityOptions(entityType);
		};
	}

	function createEntityClearButton() {
		var button = document.createElement('input');

		button.setAttribute('type', 'button');
		button.value = 'Clear';
		button.setAttribute('class', 'ui-button entity-button');
		button.addEventListener('mousedown', unfocusPlacementButtons);
		button.addEventListener('click', function(e) {
			e.currentTarget.setAttribute('class', 'ui-button entity-button selected');

			appState.currentEntityType = -1;
			appState.currentPlacementType = 'entity';

			clearEntityOptions();
		});

		return button;
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

		$container.appendChild(createEntityClearButton());

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
				row.push(1);
			}
		}

		appState.layers.push(layer);
	}

	function renderBlockType(context, blockType, rect) {
		if (blockType === BlockTypes.EMPTY) {
			return;
		}

		var asset = blockAssets[blockType];

		context.drawImage(asset, rect.x, rect.y, rect.width, rect.height);
	}

	function getTorchOffset(torch, blockWidth, blockHeight) {
		switch (torch.direction) {
			case Direction.FORWARD:
				return { x: blockWidth / 2 - 2, y: 0 };
			case Direction.LEFT:
				return { x: 0, y: blockHeight / 2 - 2 };
			case Direction.BACKWARD:
				return { x: blockWidth / 2 - 2, y: blockHeight - 5 };
			case Direction.RIGHT:
				return { x: blockWidth - 5, y: blockHeight / 2 - 2 };
		}
	}

	function getChestRect(chest, blockWidth, blockHeight) {
		switch (chest.direction) {
			case Direction.FORWARD:
			case Direction.BACKWARD:
				return {
					x: chest.x * blockWidth + blockWidth * 0.15,
					y: chest.z * blockHeight + blockHeight * 0.25,
					width: blockWidth * 0.7,
					height: blockHeight * 0.5
				};
			case Direction.LEFT:
			case Direction.RIGHT:
				return {
					x: chest.x * blockWidth + blockWidth * 0.25,
					y: chest.z * blockHeight + blockHeight * 0.15,
					width: blockWidth * 0.5,
					height: blockHeight * 0.7
				};
		}
	}

	function activeLayerEntityFilter(entity) {
		return entity.layer === appState.currentLayer;
	}

	function updateLayout() {
		$('#layout-current-layer').value = appState.currentLayer + '/' + (appState.layers.length - 1);

		var $canvas = $('#layout-canvas');
		var canvas = $canvas.getContext('2d');
		var blockWidth = $canvas.clientWidth / appState.layerSize.width;
		var blockHeight = $canvas.clientHeight / appState.layerSize.height;
		var activeLayer = appState.layers[appState.currentLayer];

		$canvas.setAttribute('width', $canvas.clientWidth);
		$canvas.setAttribute('height', $canvas.clientHeight);

		canvas.fillStyle = '#000';
		canvas.fillRect(0, 0, $canvas.clientWidth, $canvas.clientHeight);

		for (var z = 0; z < activeLayer.length; z++) {
			var row = activeLayer[z];

			for (var x = 0; x < row.length; x++) {
				var blockType = row[x];

				var rect = {
					x: blockWidth * x,
					y: blockHeight * z,
					width: blockWidth,
					height: blockHeight
				};

				renderBlockType(canvas, blockType, rect);

				if (blockType === BlockTypes.EMPTY && appState.currentLayer > 0) {
					canvas.save();
					canvas.globalAlpha = 0.3;

					var belowBlockType = appState.layers[appState.currentLayer - 1][z][x];

					if (belowBlockType === BlockTypes.EMPTY && appState.currentLayer > 1) {
						belowBlockType = appState.layers[appState.currentLayer - 2][z][x];
						canvas.globalAlpha = 0.1;
					}

					renderBlockType(canvas, belowBlockType, rect);
					canvas.restore();
				}
			}
		}

		appState.chests.filter(activeLayerEntityFilter).forEach(function(chest) {
			var asset = entityAssets[EntityTypes.CHEST];
			var rect = getChestRect(chest, blockWidth, blockHeight);

			canvas.drawImage(asset, rect.x, rect.y, rect.width, rect.height);
		});

		appState.torches.filter(activeLayerEntityFilter).forEach(function(torch) {
			var asset = entityAssets[EntityTypes.TORCH];
			var offset = getTorchOffset(torch, blockWidth, blockHeight);

			var rect = {
				x: blockWidth * torch.x + offset.x,
				y: blockHeight * torch.z + offset.y,
				width: 5,
				height: 5
			};

			canvas.drawImage(asset, rect.x, rect.y, rect.width, rect.height);
		});
	}

	function updateOutput() {
		var output = '';

		function write(string) {
			output += string;
		}

		function newline() {
			write('\n');
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

			write(`CH ${chest.layer}, ${chest.x}, ${chest.z}, ${chest.direction}, ${chest.itemType}`)
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

	function resetLevelData() {
		appState.layers.length = 0;
		appState.torches.length = 0;
		appState.chests.length = 0;
	}

	function parseOutput() {
		var value = $('#output-textarea').value;
		var lines = value.split('\n');

		function parseLayerSize(data) {
			var values = data.split(',');

			appState.layerSize.width = parseInt(values[0]);
			appState.layerSize.height = parseInt(values[1]);
		}

		function parseSpawnPosition(data) {
			var values = data.split(',');

			appState.spawn.layer = parseInt(values[0]);
			appState.spawn.x = parseInt(values[1]);
			appState.spawn.z = parseInt(values[2]);
			appState.spawn.direction = parseInt(values[3]);
		}

		function parseAmbientLight(data) {
			var values = data.split(',');

			appState.ambientLight.color.R = parseInt(values[0]);
			appState.ambientLight.color.G = parseInt(values[1]);
			appState.ambientLight.color.B = parseInt(values[2]);
			appState.ambientLight.vector.x = parseFloat(values[3]);
			appState.ambientLight.vector.y = parseFloat(values[4]);
			appState.ambientLight.vector.z = parseFloat(values[5]);
			appState.ambientLight.power = parseFloat(values[6]);
		}

		function parseVisibility(data) {
			appState.visibility = parseInt(data);
		}

		function parseBrightness(data) {
			appState.brightness = parseFloat(data);
		}

		function parseTorch(data) {
			var values = data.split(',');

			appState.torches.push({
				layer: parseInt(values[0]),
				x: parseInt(values[1]),
				z: parseInt(values[2]),
				direction: parseInt(values[3])
			});
		}

		function parseChest(data) {
			var values = data.split(',');

			appState.chests.push({
				layer: parseInt(values[0]),
				x: parseInt(values[1]),
				z: parseInt(values[2]),
				direction: parseInt(values[3]),
				itemType: parseInt(values[4])
			});
		}

		function parseLayerRow(data, activeLayer, rowIndex) {
			if (activeLayer >= appState.layers.length) {
				addLayer();
			}

			var row = appState.layers[activeLayer][rowIndex];
			var blocks = data.split(',');

			for (var i = 0; i < blocks.length; i++) {
				row[i] = parseInt(blocks[i]);
			}
		}

		resetLevelData();

		var i = 0;
		var activeLayer = 0;

		while (i < lines.length) {
			var parts = lines[i].split(/\s(.+)/);
			var label = parts[0];
			var data = parts[1];

			switch (label) {
				case 'LS':
					parseLayerSize(data);
					break;
				case 'SP':
					parseSpawnPosition(data);
					break;
				case 'AL':
					parseAmbientLight(data);
					break;
				case 'V':
					parseVisibility(data);
					break;
				case 'B':
					parseBrightness(data);
					break;
				case 'T':
					parseTorch(data);
					break;
				case 'CH':
					parseChest(data);
					break;
				case 'L':
					for (var n = 1; n <= appState.layerSize.height; n++) {
						var data = lines[i + n];

						parseLayerRow(data, activeLayer, n - 1);
					}

					activeLayer++;
					i += appState.layerSize.height;
					break;
			}

			i++;
		}

		syncSettingsInputs();
		updateLayout();
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

	function getTileCoordinate(mouseX, mouseY) {
		var $canvas = $('#layout-canvas');
		var canvasBounds = $canvas.getBoundingClientRect();
		var x = mouseX - canvasBounds.x;
		var y = mouseY - canvasBounds.y;

		return {
			x: Math.floor(x / ($canvas.clientWidth / appState.layerSize.width)),
			z: Math.floor(y / ($canvas.clientHeight / appState.layerSize.height))
		};
	}

	function clearEntities(tile) {
		function predicate(entity) {
			return entity.layer != appState.currentLayer || entity.x != tile.x || entity.z != tile.z;
		}

		appState.torches = appState.torches.filter(predicate);
		appState.chests = appState.chests.filter(predicate);
	}

	function placeCurrentEntity(tile) {
		var entityType = appState.currentEntityType;

		if (entityType === -1) {
			clearEntities(tile);
			return;
		}

		var options = appState.currentEntityOptions;

		switch (entityType) {
			case EntityTypes.TORCH:
				appState.torches.push({
					layer: appState.currentLayer,
					x: tile.x,
					z: tile.z,
					direction: options.direction
				});
				break;
			case EntityTypes.CHEST:
				appState.chests.push({
					layer: appState.currentLayer,
					x: tile.x,
					z: tile.z,
					direction: options.direction,
					itemType: options.itemType
				});
				break;
		}
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

		$('#layout-canvas').addEventListener('mousemove', function(e) {
			var tile = getTileCoordinate(e.clientX, e.clientY);

			$('#layout-current-coords').value = `${tile.x}, ${tile.z}`;
		});

		$('#layout-canvas').addEventListener('mouseleave', function() {
			$('#layout-current-coords').value = '';
		});

		$('#layout-canvas').addEventListener('click', function(e) {
			var tile = getTileCoordinate(e.clientX, e.clientY);

			if (appState.currentPlacementType === 'block') {
				appState.layers[appState.currentLayer][tile.z][tile.x] = appState.currentBlockType;
			} else {
				placeCurrentEntity(tile);
			}

			updateLayout();
			updateOutput();
		});

		$('#output-textarea').addEventListener('paste', function(){
			setTimeout(function(){
				parseOutput();
				$('#output-textarea').scrollTop = 0;
			}, 20);
		});
	}

	function preloadAssets() {
		var blocks = Object.keys(blockMap);

		// Add a null slot for empty blocks
		blockAssets.push(null);

		var totalLoaded = 0;

		// Start from index 1, skipping the empty block
		for (var i = 1; i < blocks.length; i++) {
			var block = blockMap[i];
			var filePath = `./BlockAssets/${block.file}`;

			var image = new Image();
			image.src = filePath;
			image.onload = function() {
				totalLoaded++;
			};

			blockAssets.push(image);
		}

		for (var i = 0; i < entities.length; i++) {
			var entity = entities[i];
			var filePath = `./EntityAssets/${entity.file}`;

			var image = new Image();
			image.src = filePath;
			image.onload = function() {
				totalLoaded++;
			};

			entityAssets.push(image);
		}

		return {
			then: function(callback) {
				var checkInterval = setInterval(function(){
					// Count toward all blocks except the empty block + all entities
					if (totalLoaded === (blocks.length - 1) + entities.length) {
						callback();
						clearInterval(checkInterval);
					}
				}, 100);
			}
		};
	}

	function initializeEditor() {
		for (var i = 0; i < 3; i++) {
			addLayer();
		}

		syncSettingsInputs();
		createBlockButtons();
		createEntityButtons();
		bindEvents();

		preloadAssets().then(function(){
			updateLayout();
			updateOutput();
		});

		$$('.block-button')[0].click();
	}

	window.initializeEditor = initializeEditor;
})();
