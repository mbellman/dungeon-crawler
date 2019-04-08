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

	var appState = {
		layerSize: {
			width: 10,
			height: 10
		},
		layers: 2,
		currentPlacementType: 'block',
		currentBlockType: 0,
		currentEntityType: 0,
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

	function initializeEditor() {
		createBlockButtons();
		createEntityButtons();
	}

	window.initializeEditor = initializeEditor;
})();
