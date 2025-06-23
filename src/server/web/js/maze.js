class Point {
  constructor(row, col) {
    this.row = row;
    this.col = col;
  }
}
const canvas = document.getElementById('mazeCanvas');
const ctx = canvas.getContext('2d');
let maze = {rows: 20, cols: 30};
let vWalls = [];
let hWalls = [];
let start = new Point(0, 0);
let end = new Point(19, 29);
let path = [];


async function fetchMazeFromServer() {
  const response = await fetch('http://localhost:8080/generate', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({rows: maze.rows, cols: maze.cols})
  });
  if (!response.ok) {
    throw new Error('Can not get maze from server');
  }
  const data = await response.json();
  vWalls = data.verticals.map(v => BigInt(v));
  hWalls = data.horizontals.map(h => BigInt(h));

  updateInputLimitsFromMaze();
  updateInputsFromMaze();
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  const cellWidth = canvas.width / maze.cols;
  const cellHeight = canvas.height / maze.rows;
  const size = Math.min(cellWidth, cellHeight) * 0.5;

  drawPoint(ctx, start.row, start.col, cellWidth, cellHeight, size, 'green');
  drawPoint(ctx, end.row, end.col, cellWidth, cellHeight, size, 'red');
}

async function fetchPathFromServer() {
  const response = await fetch('http://localhost:8080/pass', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({
      rows: maze.rows,
      cols: maze.cols,
      start: [start.row, start.col],
      end: [end.row, end.col],
      verticals: vWalls.map(v => v.toString()),
      horizontals: hWalls.map(h => h.toString())
    })
  });
  if (!response.ok) {
    throw new Error('Cannot get path from server');
  }
  const data = await response.json();
  path = data.pass;
  animatePath(path, maze.rows, maze.cols);
}

function animatePath(path, rows, cols) {
  const cellWidth = canvas.width / cols;
  const cellHeight = canvas.height / rows;
  const size = Math.min(cellWidth, cellHeight) * 0.5;

  drawMaze(vWalls, hWalls, rows, cols);
  drawPoint(ctx, start.row, start.col, cellWidth, cellHeight, size, 'green');
  drawPoint(ctx, end.row, end.col, cellWidth, cellHeight, size, 'red');

  let step = 1;
  function drawNextStep() {
    if (step >= path.length) return;
    const [row, col] = path[step];
    drawPoint(ctx, row, col, cellWidth, cellHeight, size, 'red');
    step++;
    setTimeout(drawNextStep, 10);
  }
  drawNextStep();
}


function drawMaze(vWalls, hWalls, rows, cols) {
  const cellWidth = canvas.width / cols;
  const cellHeight = canvas.height / rows;
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.strokeStyle = 'black';
  ctx.lineWidth = 1;
  ctx.strokeRect(0.5, 0.5, canvas.width - 1, canvas.height - 1);

  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols - 1; j++) {
      if ((vWalls[i] & (1n << BigInt(j))) !== 0n) {
        ctx.beginPath();
        ctx.moveTo((j + 1) * cellWidth + 0.5, i * cellHeight + 0.5);
        ctx.lineTo((j + 1) * cellWidth + 0.5, (i + 1) * cellHeight + 0.5);
        ctx.stroke();
      }
    }
  }

  for (let i = 0; i < rows - 1; i++) {
    for (let j = 0; j < cols; j++) {
      if ((hWalls[i] & (1n << BigInt(j))) !== 0n) {
        ctx.beginPath();
        ctx.moveTo(j * cellWidth + 0.5, (i + 1) * cellHeight + 0.5);
        ctx.lineTo((j + 1) * cellWidth + 0.5, (i + 1) * cellHeight + 0.5);
        ctx.stroke();
      }
    }
  }
}

function drawPoint(ctx, row, col, cellWidth, cellHeight, size, color) {
  const x = (col + 0.5) * cellWidth - size / 2;
  const y = (row + 0.5) * cellHeight - size / 2;
  ctx.fillStyle = color;
  ctx.fillRect(x, y, size, size);
}

function onStartInputChange() {
  const row = parseInt(document.getElementById('startRow').value);
  const col = parseInt(document.getElementById('startCol').value);
  if (isNaN(row) || isNaN(col)) return;
  if (row < 1 || row > maze.rows || col < 1 || col > maze.cols) return;
  start = new Point(row - 1, col - 1);
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  const cellWidth = canvas.width / maze.cols;
  const cellHeight = canvas.height / maze.rows;
  const size = Math.min(cellWidth, cellHeight) * 0.5;
  drawPoint(ctx, start.row, start.col, cellWidth, cellHeight, size, 'green');
  drawPoint(ctx, end.row, end.col, cellWidth, cellHeight, size, 'red');
  path = [];
}

function onEndInputChange() {
  const row = parseInt(document.getElementById('endRow').value);
  const col = parseInt(document.getElementById('endCol').value);
  if (isNaN(row) || isNaN(col)) return;
  if (row < 1 || row > maze.rows || col < 1 || col > maze.cols) return;
  end = new Point(row - 1, col - 1);
  drawMaze(vWalls, hWalls, maze.rows, maze.cols);
  const cellWidth = canvas.width / maze.cols;
  const cellHeight = canvas.height / maze.rows;
  const size = Math.min(cellWidth, cellHeight) * 0.5;
  drawPoint(ctx, start.row, start.col, cellWidth, cellHeight, size, 'green');
  drawPoint(ctx, end.row, end.col, cellWidth, cellHeight, size, 'red');
  path = [];
}

function updateInputLimitsFromMaze() {
  document.getElementById('startRow').max = maze.rows;
  document.getElementById('startCol').max = maze.cols;
  document.getElementById('endRow').max = maze.rows;
  document.getElementById('endCol').max = maze.cols;
}



document.addEventListener('DOMContentLoaded', function() {
  resetInputsToDefaults();

  function updateMazeParams() {
    const rows = parseInt(document.getElementById('rows').value);
    const cols = parseInt(document.getElementById('cols').value);
    maze = {rows, cols};
    start = new Point(0, 0);
    end = new Point(rows - 1, cols - 1);
  }
  updateMazeParams();
  fetchMazeFromServer()

  document.getElementById('generateMaze').onclick = async function() {
    try {
      updateMazeParams();
      await fetchMazeFromServer();
    } catch (error) {
      alert('Can not generate maze: ' + error.message);
    }
  };
  document.getElementById('startRow')
      .addEventListener('input', onStartInputChange);
  document.getElementById('startCol')
      .addEventListener('input', onStartInputChange);
  document.getElementById('endRow').addEventListener('input', onEndInputChange);
  document.getElementById('endCol').addEventListener('input', onEndInputChange);
  document.getElementById('solveMaze').onclick = async function() {
    if (!start || !end) {
      alert('Set start and end points!');
      return;
    }
    try {
      await fetchPathFromServer();
    } catch (err) {
      alert('Can not get path: ' + err.message);
    }
  };
});

function resetInputsToDefaults() {
  document.getElementById('rows').value = 25;
  document.getElementById('cols').value = 50;
  document.getElementById('startRow').value = 1;
  document.getElementById('startCol').value = 1;
  document.getElementById('endRow').value = 25;
  document.getElementById('endCol').value = 50;
}

const loadButton = document.getElementById('Load');
const fileInput = document.getElementById('fileInput');

loadButton.addEventListener('click', () => {
  fileInput.click();
});

fileInput.addEventListener('change', (event) => {
  const file = event.target.files[0];
  if (!file) return;
  if (!file.name.endsWith('.txt')) {
    alert('Please select a .txt file');
    return;
  }
  if (file.size > 12 * 1024) {
    alert('File is too large. Maximum allowed size is 12 KB.');
    return;
  }
  const reader = new FileReader();
  reader.onload = function(e) {
    const content = e.target.result;
    loadMaze(content);
  };
  reader.readAsText(file);
});

function loadMaze(content) {
  const lines = content.trim().split('\n');
  const [rowsStr, colsStr] = lines[0].trim().split(' ');
  const rows = Number(rowsStr);
  const cols = Number(colsStr);

  if (!Number.isInteger(rows) || !Number.isInteger(cols)) {
    alert('Rows and columns must be integers');
    return;
  }
  if (rows < 1 || rows > 50 || cols < 1 || cols > 50) {
    alert('Rows and columns must be between 1 and 50');
    return;
  }
  let result = loadMatrixNumber(lines, 1, rows, cols - 1);
  if (!result.success) {
    alert('Can not load vertical walls');
    return;
  }
  const vWallsMatrix = result.matrix;

  result = loadMatrixNumber(lines, result.nextIndex, rows, cols);
  if (!result.success) {
    alert('Can not load horizontal walls');
    return;
  }
  const hWallsMatrix = result.matrix;

  maze.rows = rows;
  maze.cols = cols;

  vWalls = vWallsMatrix;
  hWalls = hWallsMatrix;

  start = new Point(0, 0);
  end = new Point(rows - 1, cols - 1);

  drawMaze(vWalls, hWalls, rows, cols);

  const cellWidth = canvas.width / cols;
  const cellHeight = canvas.height / rows;
  const size = Math.min(cellWidth, cellHeight) * 0.5;

  drawPoint(ctx, start.row, start.col, cellWidth, cellHeight, size, 'green');
  drawPoint(ctx, end.row, end.col, cellWidth, cellHeight, size, 'red');
  updateInputsFromMaze();
  path = [];
}

function loadMatrixNumber(lines, startIndex, rows, cols) {
  const matrix = [];
  let i = 0;
  let lineIndex = startIndex;
  while (i < rows && lineIndex < lines.length) {
    let line = lines[lineIndex].trim();
    lineIndex++;
    if (line.length !== 0) {
      const parts = line.split(/\s+/);
      if (parts.length < cols) {
        return {matrix: null, success: false, nextIndex: lineIndex};
      }
      let value = 0n;
      for (let j = 0; j < cols; j++) {
        if (parts[j] === '1') {
          value |= (1n << BigInt(j));
        } else if (parts[j] != '0') {
          return {matrix: null, success: false, nextIndex: lineIndex};
        }
      }
      matrix.push(value);
      i++;
    }
  }
  if (i < rows) {
    return {matrix: null, success: false, nextIndex: lineIndex};
  }
  return {matrix, success: true, nextIndex: lineIndex};
}

function updateInputsFromMaze() {
  document.getElementById('rows').value = maze.rows;
  document.getElementById('cols').value = maze.cols;
  document.getElementById('startRow').value = start.row + 1;
  document.getElementById('startCol').value = start.col + 1;
  document.getElementById('endRow').value = end.row + 1;
  document.getElementById('endCol').value = end.col + 1;
}

function bitmaskToMatrix(bitmask, rows, cols) {
  const matrix = [];
  for (let i = 0; i < rows; i++) {
    const row = [];
    for (let j = 0; j < cols; j++) {
      row.push(((bitmask[i] >> BigInt(j)) & 1n) !== 0n ? 1 : 0);
    }
    matrix.push(row);
  }
  return matrix;
}

document.getElementById('Save').addEventListener('click', () => {
  function matrixToString(matrix) {
    return matrix.map(row => row.join(' ')).join('\n');
  }
  const vWallsMatrix = bitmaskToMatrix(vWalls, maze.rows, maze.cols);
  const hWallsMatrix = bitmaskToMatrix(hWalls, maze.rows, maze.cols);
  let content = `${maze.rows} ${maze.cols}\n`;
  content += matrixToString(vWallsMatrix) + '\n\n';
  content += matrixToString(hWallsMatrix) + '\n';
  const timestamp = Date.now();
  const filename = `maze_${timestamp}.txt`;
  const blob = new Blob([content], {type: 'text/plain'});
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = filename;
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  URL.revokeObjectURL(url);
});
