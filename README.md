# Android Linux 4.9.337 Kernel for Xiaomi Redmi 4 Prime (`markw`)

适用于小米红米 4 高配版（**Redmi 4 Prime / `markw`**, Qualcomm Snapdragon 625 `MSM8953`）的 **Linux `4.9.337`** 内核源码仓库，针对采用 **First-Stage Init (SAR)** 分区布局的 Android 14 ROM（如 `crDroidAndroid-14.0-20240803-markw-v10.6.zip`）适配了 **KernelSU-Next**（Manual Hook）与 **SUSFS `v1.5.5`**。

---

## 分支说明 (Branches)

| 分支 | 说明 |
| :--- | :--- |
| **`crdroid-14-ksun-susfs`** | **推荐主用分支**：集成 **KernelSU-Next (`v3.4.0-legacy` / `UAPI 6`, Manual Hook)** + **SUSFS `v1.5.5` (`NON-GKI`)**（完整开启 13 项内核级隐藏与卸载特性） |
| **`crdroid-14-ksun`** | 纯净稳定分支：仅集成 **KernelSU-Next (`v3.4.0-legacy` / `UAPI 6`, Manual Hook)**，不含 SUSFS 补丁 |
| `master` / `OldBase` | 上游基础源码分支（保留历史基线） |

---

## 配套管理器与 SUSFS 模块 (Manager & SUSFS Module)

### 1. KernelSU-Next 管理器
- 内核 `UAPI` 版本为 **`6`**（`v3.4.0-legacy`, versionCode `33327`），请搭配支持 `UAPI 6` 的 **KernelSU-Next Manager**（如 Release 附件提供的 `KernelSU_Next_v3.4.2-199-gaa235b99_33554-release.apk` 或更新版本）。
- 原版 **KernelSU-Next** 主页默认不显示 SUSFS 状态栏，但内核底层的自动挂载隐藏（`AUTO_ADD_SUS_MOUNT`）、非 Root 应用自动卸载（`AUTO_ADD_TRY_UMOUNT`）与 `/proc/kallsyms` 符号隐藏在开机后已默认生效。

### 2. SUSFS 图形化管理模块（WebUI）
SUSFS 模块与原版 KernelSU / KernelSU-Next 完全通用。若需在管理器中直观查看 **SUSFS `v1.5.5`** 状态或配置伪装 `uname`、自定义 `sus_path`、`open_redirect` 等功能，请下载安装社区维护的 WebUI 模块：

- **SUSFS4KSU 模块下载地址**：https://github.com/sidex15/susfs4ksu-module/releases
  - 推荐下载 **`v1.5.2+_R28`**（或更高版本）中的 `ksu_module_susfs_1.5.2+.zip`（内置 Universal `ksu_susfs` 二进制，自动识别本内核的 `SUSFS v1.5.5` `NON-GKI` / `prctl` 接口）。
  - 在 **KernelSU-Next → 模块** 页面从本地安装并重启后，点击模块卡片上的 **「WebUI」** 即可打开图形化控制面板。

---

## 已启用的 SUSFS `v1.5.5` 内核特性 (`crdroid-14-ksun-susfs`)

- `CONFIG_KSU_SUSFS_HAS_MAGIC_MOUNT=y`（适配 KernelSU-Next Magic Mount）
- `CONFIG_KSU_SUSFS_SUS_PATH=y`
- `CONFIG_KSU_SUSFS_SUS_MOUNT=y`
- `CONFIG_KSU_SUSFS_AUTO_ADD_SUS_KSU_DEFAULT_MOUNT=y`
- `CONFIG_KSU_SUSFS_AUTO_ADD_SUS_BIND_MOUNT=y`
- `CONFIG_KSU_SUSFS_SUS_KSTAT=y`
- `CONFIG_KSU_SUSFS_TRY_UMOUNT=y`
- `CONFIG_KSU_SUSFS_AUTO_ADD_TRY_UMOUNT_FOR_BIND_MOUNT=y`
- `CONFIG_KSU_SUSFS_SPOOF_UNAME=y`
- `CONFIG_KSU_SUSFS_ENABLE_LOG=y`
- `CONFIG_KSU_SUSFS_HIDE_KSU_SUSFS_SYMBOLS=y`
- `CONFIG_KSU_SUSFS_SPOOF_CMDLINE_OR_BOOTCONFIG=y`
- `CONFIG_KSU_SUSFS_OPEN_REDIRECT=y`

---

## 刷入注意事项 (`markw` `V8.0.5.0` Bootloader)

> [!IMPORTANT]
> 红米 4 高配版官方 `V8.0.5.0` `aboot` 会校验 `boot.img` 尾部的 4096 字节特定后缀（`0x30 0x83 0x19 0x89 0x64` + 4091 字节 `0x00`）。
> - **请务必刷入 Release 中带 `-appended.img` 后缀的镜像**（如 `boot-ksun-susfs-appended.img`），切勿直接刷入未追加签名的裸 `boot.img`，否则重启会直接卡 Fastboot。
> - 本内核针对 **First-Stage Init (SAR)** 分区表构建（`/dev/block/mmcblk0p24` 挂载 `/`，`cust` `mmcblk0p48` 挂载 `/vendor`），请勿混刷至采用 Second-Stage Init / Dynamic Retrofit 的 Android 15/16 ROM。

---

## 编译简述 (Build Instructions)

使用 Android Clang 17 (`clang-r487747c`) 配合 `aarch64-linux-android-4.9` / `arm-linux-androideabi-4.9` 编译：

```bash
export ARCH=arm64
export SUBARCH=arm64
export PATH="/path/to/clang-r487747c/bin:/path/to/gcc-arm64/bin:/path/to/gcc-arm/bin:$PATH"

make O=out ARCH=arm64 CC=clang LLVM=1 LLVM_IAS=1 \
  CLANG_TRIPLE=aarch64-linux-gnu- \
  CROSS_COMPILE=aarch64-linux-android- \
  CROSS_COMPILE_ARM32=arm-linux-androideabi- \
  markw_defconfig

make -j"$(nproc)" O=out ARCH=arm64 CC=clang LLVM=1 LLVM_IAS=1 \
  CLANG_TRIPLE=aarch64-linux-gnu- \
  CROSS_COMPILE=aarch64-linux-android- \
  CROSS_COMPILE_ARM32=arm-linux-androideabi- \
  Image.gz-dtb
```

编译产物位于 `out/arch/arm64/boot/Image.gz-dtb`，替换进原包 `boot.img` 后需追加 4096 字节 `markw` `aboot` 尾部标识再刷入 `boot` 分区。
