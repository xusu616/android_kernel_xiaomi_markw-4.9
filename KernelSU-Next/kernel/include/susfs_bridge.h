#ifndef __KSU_H_SUSFS_BRIDGE
#define __KSU_H_SUSFS_BRIDGE

#include <linux/types.h>
#include <linux/stat.h>

#ifdef CONFIG_KSU_SUSFS
#include <linux/susfs.h>

extern bool susfs_is_umount_for_zygote_system_process_enabled;
extern bool susfs_is_log_enabled;

bool susfs_is_current_ksu_domain(void);
bool susfs_is_current_zygote_domain(void);
bool susfs_is_current_init_domain(void);
bool susfs_is_allow_su(void);
void susfs_on_post_fs_data(void);
int ksu_susfs_handle_prctl(int option, unsigned long arg2, unsigned long arg3,
			   unsigned long arg4, unsigned long arg5);
#endif /* CONFIG_KSU_SUSFS */

#endif /* __KSU_H_SUSFS_BRIDGE */
