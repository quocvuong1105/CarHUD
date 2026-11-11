```javascript
// ======================================================
// CARHUD - ESP32-S3 WEB FLASHER
// Platform: ESP32-S3
// Board variants: ES3C28P / ES3N28P
// ======================================================

// 1. CONFIGURATION
const OWNER = "quocvuong1105";
const REPO = "CarHUD";

const API =
  `https://api.github.com/repos/${OWNER}/${REPO}/releases`;

const FIRMWARE_BASE = "./firmware/";

const BOARD_FIRMWARE = {
  ES3C28P: "CarHUD-ES3C28P-merged.bin"
};

// 2. DOM ELEMENTS
const releaseSelect = document.querySelector("#release");
const installer = document.querySelector("#installer");
const info = document.querySelector("#firmware-info");
const note = document.querySelector("#release-note");

if (!releaseSelect || !installer || !info || !note) {
  throw new Error(
    "Không tìm thấy một hoặc nhiều thành phần HTML cần thiết. Kiểm tra ID trong index.html."
  );
}

// 3. APPLICATION STATE
let releases = [];
let selectedBoard = "ES3C28P";
let manifestUrl = null;
let updateSequence = 0;

// 4. UI HELPERS
function setInfo(message, status = "info") {
  const paragraph = info.querySelector("p");

  if (paragraph) {
    paragraph.textContent = message;
  }

  info.dataset.status = status;
}

function selectedRelease() {
  return releases.find(
    release => release.id === Number(releaseSelect.value)
  );
}

// 5. MANIFEST HELPERS
function clearManifest() {
  installer.removeAttribute("manifest");

  if (manifestUrl) {
    URL.revokeObjectURL(manifestUrl);
    manifestUrl = null;
  }
}

function firmwareUrlFor(release) {
  const expectedFile = BOARD_FIRMWARE[selectedBoard];

  const asset = release.assets?.find(
    item => item.name === expectedFile && item.size > 0
  );

  if (!asset) {
    throw new Error(`Không tìm thấy firmware ${expectedFile}`);
  }

  return asset.browser_download_url;
}

// 6. UPDATE MANIFEST
async function updateManifest() {
  const currentSequence = ++updateSequence;

  clearManifest();

  const release = selectedRelease();

  if (!release) {
    setInfo(
      "Chưa chọn phiên bản firmware.",
      "warning"
    );
    return;
  }

  const expectedFile = BOARD_FIRMWARE[selectedBoard];

  if (!expectedFile) {
    setInfo(
      `Chưa cấu hình firmware cho bo mạch ${selectedBoard}.`,
      "warning"
    );
    return;
  }

  const asset = release.assets?.find(
    item =>
      item.name === expectedFile &&
      item.size > 0
  );

  if (!asset) {
    setInfo(
      `Release ${release.tag_name} chưa có file ${expectedFile}.`,
      "warning"
    );
    return;
  }

  const firmwareUrl = firmwareUrlFor(release);

  const manifest = {
    name: `CarHUD ${selectedBoard}`,
    version: release.tag_name,
    new_install_prompt_erase: true,
    builds: [
      {
        chipFamily: "ESP32-S3",
        parts: [
          {
            path: firmwareUrl,
            offset: 0
          }
        ]
      }
    ]
  };

  const blob = new Blob(
    [JSON.stringify(manifest, null, 2)],
    {
      type: "application/json"
    }
  );

  const newManifestUrl = URL.createObjectURL(blob);

  manifestUrl = newManifestUrl;

  try {
    await customElements.whenDefined(
      "esp-web-install-button"
    );

    // Bỏ qua manifest cũ nếu người dùng đã đổi phiên bản.
    if (currentSequence !== updateSequence) {
      URL.revokeObjectURL(newManifestUrl);
      return;
    }

    // Kiểm tra manifest Blob có đọc được không.
    const testResponse = await fetch(newManifestUrl);

    if (!testResponse.ok) {
      throw new Error(
        `Không đọc được manifest: HTTP ${testResponse.status}`
      );
    }

    await testResponse.json();

    if (currentSequence !== updateSequence) {
      URL.revokeObjectURL(newManifestUrl);
      return;
    }

    // Gán manifest cho ESP Web Tools.
    installer.manifest = newManifestUrl;

    const sizeMB = (
      asset.size / 1024 / 1024
    ).toFixed(2);

    setInfo(
      `${release.tag_name} · ${selectedBoard} · ${sizeMB} MB. Firmware đã sẵn sàng.`,
      "success"
    );

    console.log(
      "[CarHUD] Manifest:",
      manifest
    );

    console.log(
      "[CarHUD] Manifest URL:",
      newManifestUrl
    );

    console.log(
      "[CarHUD] Firmware URL:",
      firmwareUrl
    );

  } catch (error) {
    if (currentSequence !== updateSequence) {
      return;
    }

    console.error(
      "[CarHUD] Manifest initialization error:",
      error
    );

    clearManifest();

    setInfo(
      `Không thể khởi tạo manifest: ${error.message}`,
      "error"
    );
  }
}

// 7. LOAD GITHUB RELEASES
async function loadReleases() {
  releaseSelect.disabled = true;

  note.textContent =
    "Đang kết nối GitHub Releases...";

  setInfo(
    "Đang tải danh sách phiên bản firmware...",
    "info"
  );

  clearManifest();

  try {
    console.log(
      "[CarHUD] Loading releases:",
      API
    );

    const response = await fetch(API, {
      headers: {
        Accept: "application/vnd.github+json"
      },
      cache: "no-store"
    });

    if (!response.ok) {
      if (response.status === 404) {
        throw new Error(
          "Không tìm thấy GitHub Releases. Kiểm tra repository và quyền truy cập."
        );
      }

      if (response.status === 403) {
        throw new Error(
          "GitHub API đang giới hạn truy cập. Hãy thử lại sau."
        );
      }

      throw new Error(
        `GitHub API lỗi HTTP ${response.status}.`
      );
    }

    const data = await response.json();

    if (!Array.isArray(data)) {
      throw new Error(
        "Dữ liệu GitHub Releases không hợp lệ."
      );
    }

    releases = data
      .filter(release =>
        !release.draft &&
        !release.prerelease
      )
      .sort((a, b) =>
        new Date(b.published_at) -
        new Date(a.published_at)
      );

    releaseSelect.innerHTML = "";

    if (releases.length === 0) {
      throw new Error(
        "Chưa có phiên bản firmware hợp lệ."
      );
    }

    // Thêm danh sách phiên bản.
    releases.forEach(release => {
      const option = document.createElement("option");

      option.value = String(release.id);

      const publishedDate = release.published_at
        ? new Date(
            release.published_at
          ).to
      ).toLocaleDateString("vi-VN")
    : "";

  option.textContent =
    `${release.tag_name} · ${publishedDate}`;

  releaseSelect.appendChild(option);
});

releaseSelect.value = String(releases[0].id);
releaseSelect.disabled = false;

note.textContent =
  `Tìm thấy ${releases.length} phiên bản.`;

await updateManifest();

} catch (error) {
  console.error("[CarHUD] Load releases error:", error);

  releases = [];
  releaseSelect.innerHTML = "";

  releaseSelect.appendChild(
    new Option("Không có phiên bản khả dụng", "")
  );

  releaseSelect.disabled = true;

  setInfo(error.message, "error");
  note.textContent = error.message;
}
}

releaseSelect.addEventListener("change", updateManifest);

document
  .querySelectorAll('input[name="board"]')
  .forEach(input => {
    input.addEventListener("change", () => {
      if (!input.checked) return;

      selectedBoard = input.value;
      updateManifest();
    });
  });

initializeFlasher();
loadReleases();