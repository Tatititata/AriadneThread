class Cave {
  constructor(rows, cols) {
    this.rows = rows;
    this.cols = cols;
    this.v_w = Array(rows).fill().map(() => Array(cols).fill(0));
    this.h_w = Array(rows).fill().map(() => Array(cols).fill(0));
  }
}

function generateCave(cave) {
  const chance = parseFloat(document.getElementById('chance').value);
  for (let i = 0; i < cave.rows; i++) {
    for (let j = 0; j < cave.cols; j++) {
      cave.v_w[i][j] = Math.random() < chance ? 1 : 0;
    }
  }
}

function proceedAlive(cave, i, j, death) {
  let sum = -1;
  for (let di = -1; di <= 1; di++) {
    const new_i = i + di;
    for (let dj = -1; dj <= 1; dj++) {
      const new_j = j + dj;
      if (new_i < 0 || new_i >= cave.rows || new_j < 0 || new_j >= cave.cols) {
        sum++;
      } else {
        sum += cave.v_w[new_i][new_j];
      }
    }
  }
  cave.h_w[i][j] = sum >= death ? 1 : 0;
}

function proceedDead(cave, i, j, birth) {
  let sum = 0;
  for (let di = -1; di <= 1; di++) {
    const new_i = i + di;
    for (let dj = -1; dj <= 1; dj++) {
      const new_j = j + dj;
      if (new_i < 0 || new_i >= cave.rows || new_j < 0 || new_j >= cave.cols) {
        sum++;
      } else {
        sum += cave.v_w[new_i][new_j];
      }
    }
  }
  cave.h_w[i][j] = sum > birth ? 1 : 0;
}

function evolveCave(cave, birth, death) {
  let changed = false;
  for (let i = 0; i < cave.rows; i++) {
    for (let j = 0; j < cave.cols; j++) {
      if (cave.v_w[i][j]) {
        proceedAlive(cave, i, j, death);
      } else {
        proceedDead(cave, i, j, birth);
      }
      if (cave.v_w[i][j] !== cave.h_w[i][j]) {
        changed = true;
      }
    }
  }

  for (let i = 0; i < cave.rows; i++) {
    for (let j = 0; j < cave.cols; j++) {
      cave.v_w[i][j] = cave.h_w[i][j];
    }
  }
  return !changed;
}

function drawCave(cave) {
  const canvas = document.getElementById('mazeCanvas');
  const ctx = canvas.getContext('2d');
  const cellWidth = canvas.width / cave.cols;
  const cellHeight = canvas.height / cave.rows;

  ctx.clearRect(0, 0, canvas.width, canvas.height);

  for (let i = 0; i < cave.rows; i++) {
    for (let j = 0; j < cave.cols; j++) {
      ctx.fillStyle = cave.v_w[i][j] ? '#000' : '#fff';
      ctx.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight);
    }
  }
}

let cave = new Cave(10, 10);
let maxSteps = 7 + Math.floor((cave.rows + cave.cols) / 3);
const chance = 0.45;

document.getElementById('generateMaze').onclick = function() {
  stopAnimation();
  const rows = parseInt(document.getElementById('rows').value);
  const cols = parseInt(document.getElementById('cols').value);
  cave = new Cave(rows, cols);
  generateCave(cave);
  drawCave(cave);
  maxSteps = 7 + Math.floor((cave.rows + cave.cols) / 3);
};

let isAnimating = false;
let animationId = null;

function animateCave(cave, birth, death, currentStep = 0) {
  if (!isAnimating || currentStep >= maxSteps) {
    isAnimating = false;
    document.getElementById('solveMaze').textContent = 'Solve cave';
    return;
  }
  const stable = evolveCave(cave, birth, death);
  drawCave(cave);
  if (stable) {
    isAnimating = false;
    document.getElementById('solveMaze').textContent = 'Solve cave';
    return;
  }
  const ms = parseInt(document.getElementById('delayMs').value) || 200;
  animationId = setTimeout(() => {
    animateCave(cave, birth, death, currentStep + 1);
  }, ms);
}

function stopAnimation() {
  if (animationId) {
    clearTimeout(animationId);
    animationId = null;
    isAnimating = false;
    document.getElementById('solveMaze').textContent = 'Solve cave';
  }
}

document.getElementById('solveMaze').onclick = function() {
  if (isAnimating) {
    stopAnimation();
  } else {
    isAnimating = true;
    this.textContent = 'Stop';
    const birth = parseInt(document.getElementById('birth').value);
    const death = parseInt(document.getElementById('death').value);
    maxSteps = 7 + Math.floor((cave.rows + cave.cols) / 3);
    animateCave(cave, birth, death);
  }
};

document.getElementById('nextStep').onclick = function() {
  stopAnimation();
  const birth = parseInt(document.getElementById('birth').value);
  const death = parseInt(document.getElementById('death').value);
  evolveCave(cave, birth, death);
  drawCave(cave);
};

function resetInputsToDefaults() {
  document.getElementById('rows').value = 25;
  document.getElementById('cols').value = 50;
  document.getElementById('birth').value = 4;
  document.getElementById('death').value = 3;
  document.getElementById('chance').value = 0.45;
  document.getElementById('delayMs').value = 200;
}

document.addEventListener('DOMContentLoaded', function() {
  resetInputsToDefaults();
  const rows = parseInt(document.getElementById('rows').value);
  const cols = parseInt(document.getElementById('cols').value);
  cave = new Cave(rows, cols);
  generateCave(cave);
  drawCave(cave);
});