const replayFileInput = document.getElementById("replayFile");
const playPauseButton = document.getElementById("playPause");
const resetButton = document.getElementById("reset");
const speedInput = document.getElementById("speed");
const speedValue = document.getElementById("speedValue");
const timelineInput = document.getElementById("timeline");
const cycleValue = document.getElementById("cycleValue");
const processValue = document.getElementById("processValue");
const winnerValue = document.getElementById("winnerValue");
const legend = document.getElementById("legend");
const emptyState = document.getElementById("emptyState");
const replayStatus = document.getElementById("replayStatus");
const arenaCanvas = document.getElementById("arenaCanvas");
const context = arenaCanvas.getContext("2d");

const CELL_SIZE = 12;
const OWNER_COLORS = [
  "#3b82f6",
  "#ef4444",
  "#22c55e",
  "#f59e0b",
  "#8b5cf6",
  "#ec4899",
  "#06b6d4",
  "#f97316",
];

let replayData = null;
let currentFrameIndex = 0;
let playing = false;
let animationHandle = null;
let lastTimestamp = 0;
let accumulator = 0;

function setReplayStatus(message) {
  replayStatus.textContent = message;
}

function updateSpeedLabel() {
  speedValue.textContent = `${speedInput.value} fps`;
}

function ownerColor(ownerId) {
  if (!ownerId) {
    return "#0f172a";
  }
  return OWNER_COLORS[(ownerId - 1) % OWNER_COLORS.length];
}

function championNameById(championId) {
  if (!replayData) {
    return "Unknown";
  }
  const champion = replayData.champions.find((entry) => entry.id === championId);
  return champion ? champion.name : `Champion ${championId}`;
}

function bytesFromHex(memoryHex) {
  const bytes = [];
  let index = 0;

  while (index < memoryHex.length) {
    bytes.push(parseInt(memoryHex.slice(index, index + 2), 16));
    index += 2;
  }
  return bytes;
}

function ownersFromHex(ownerHex) {
  const owners = [];
  let index = 0;

  while (index < ownerHex.length) {
    owners.push(parseInt(ownerHex.slice(index, index + 2), 16));
    index += 2;
  }
  return owners;
}

function renderLegend() {
  legend.innerHTML = "";
  if (!replayData) {
    return;
  }
  replayData.champions.forEach((champion) => {
    const item = document.createElement("div");
    const swatch = document.createElement("span");
    const label = document.createElement("span");

    item.className = "legend-item";
    swatch.className = "swatch";
    swatch.style.backgroundColor = ownerColor(champion.id);
    label.textContent = `${champion.id}: ${champion.name} (start ${champion.start})`;

    item.appendChild(swatch);
    item.appendChild(label);
    legend.appendChild(item);
  });
}

function processPositions(frame) {
  const positions = new Map();

  frame.processes.forEach((processEntry) => {
    const existing = positions.get(processEntry.pc) || [];
    existing.push(processEntry.championId);
    positions.set(processEntry.pc, existing);
  });
  return positions;
}

function winnerLabelForFrame(frameIndex) {
  if (!replayData || !replayData.winner) {
    return "No winner";
  }
  if (frameIndex >= replayData.frames.length - 1) {
    return `${replayData.winner.id}: ${replayData.winner.name}`;
  }
  return "Battle in progress";
}

function renderFrame(frameIndex) {
  const rowSize = replayData.rowSize || 32;
  const bytes = bytesFromHex(replayData.frames[frameIndex].memory);
  const owners = ownersFromHex(replayData.frames[frameIndex].owners || "");
  const positions = processPositions(replayData.frames[frameIndex]);
  let index = 0;

  currentFrameIndex = frameIndex;
  context.clearRect(0, 0, arenaCanvas.width, arenaCanvas.height);
  while (index < bytes.length) {
    const x = (index % rowSize) * CELL_SIZE;
    const y = Math.floor(index / rowSize) * CELL_SIZE;

    context.fillStyle = ownerColor(owners[index] || 0);
    context.fillRect(x, y, CELL_SIZE - 1, CELL_SIZE - 1);

    if (positions.has(index)) {
      context.strokeStyle = "#f8fafc";
      context.lineWidth = 2;
      context.strokeRect(x + 1, y + 1, CELL_SIZE - 3, CELL_SIZE - 3);
    }
    index++;
  }

  cycleValue.textContent = String(replayData.frames[frameIndex].cycle);
  processValue.textContent = String(replayData.frames[frameIndex].aliveProcesses);
  winnerValue.textContent = winnerLabelForFrame(frameIndex);
  timelineInput.value = String(frameIndex);
}

function stopPlayback() {
  playing = false;
  playPauseButton.textContent = "Play";
  if (animationHandle !== null) {
    cancelAnimationFrame(animationHandle);
    animationHandle = null;
  }
}

function tick(timestamp) {
  const frameDuration = 1000 / Number(speedInput.value);

  if (!playing || !replayData) {
    return;
  }
  if (lastTimestamp === 0) {
    lastTimestamp = timestamp;
  }
  accumulator += timestamp - lastTimestamp;
  lastTimestamp = timestamp;
  while (accumulator >= frameDuration) {
    accumulator -= frameDuration;
    if (currentFrameIndex >= replayData.frames.length - 1) {
      stopPlayback();
      return;
    }
    renderFrame(currentFrameIndex + 1);
  }
  animationHandle = requestAnimationFrame(tick);
}

function startPlayback() {
  if (!replayData || replayData.frames.length === 0) {
    return;
  }
  if (currentFrameIndex >= replayData.frames.length - 1) {
    renderFrame(0);
  }
  playing = true;
  playPauseButton.textContent = "Pause";
  accumulator = 0;
  lastTimestamp = 0;
  animationHandle = requestAnimationFrame(tick);
}

function loadReplay(payload, sourceLabel) {
  replayData = payload;
  emptyState.style.display = "none";
  timelineInput.disabled = false;
  timelineInput.min = "0";
  timelineInput.max = String(Math.max(0, replayData.frames.length - 1));
  renderLegend();
  renderFrame(0);
  stopPlayback();
  setReplayStatus(sourceLabel);
}

async function loadReplayFromUrl(url) {
  const response = await fetch(url, { cache: "no-store" });

  if (!response.ok) {
    throw new Error("Replay fetch failed");
  }
  loadReplay(await response.json(), `Loaded bundled replay: ${url}`);
}

replayFileInput.addEventListener("change", async (event) => {
  const [file] = event.target.files || [];
  if (!file) {
    return;
  }
  loadReplay(JSON.parse(await file.text()), `Loaded local replay: ${file.name}`);
});

playPauseButton.addEventListener("click", () => {
  if (playing) {
    stopPlayback();
  } else {
    startPlayback();
  }
});

resetButton.addEventListener("click", () => {
  stopPlayback();
  if (replayData) {
    renderFrame(0);
  }
});

timelineInput.addEventListener("input", (event) => {
  stopPlayback();
  if (replayData) {
    renderFrame(Number(event.target.value));
  }
});

speedInput.addEventListener("input", () => {
  updateSpeedLabel();
  if (playing) {
    stopPlayback();
    startPlayback();
  }
});

updateSpeedLabel();

const params = new URLSearchParams(window.location.search);
if (params.has("replay")) {
  loadReplayFromUrl(params.get("replay")).catch(() => {});
} else {
  loadReplayFromUrl("latest_replay.json").catch(() => {
    setReplayStatus("No bundled replay loaded yet.");
  });
}
