#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00001814d00000301sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001814d00000302sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001814d00000401sv*sd*bc*sc*i*");
