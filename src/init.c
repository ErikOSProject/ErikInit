#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <erikbus.h>

DECLARE_SERVICE(fi_erikinkinen_kernel)
DECLARE_INTERFACE(fi_erikinkinen_kernel, fi_erikinkinen_kernel_Stdio)
DECLARE_METHOD(fi_erikinkinen_kernel, fi_erikinkinen_kernel_Stdio, Write)
DECLARE_INTERFACE(fi_erikinkinen_kernel,
		  fi_erikinkinen_kernel_GlobalNameService)
DECLARE_METHOD(fi_erikinkinen_kernel, fi_erikinkinen_kernel_GlobalNameService,
	       RegisterDestination)

DEFINE_SERVICE(fi_erikinkinen_kernel, "fi.erikinkinen.kernel")
DEFINE_INTERFACE(fi_erikinkinen_kernel, fi_erikinkinen_kernel_Stdio,
		 "fi.erikinkinen.kernel.Stdio")
DEFINE_METHOD(fi_erikinkinen_kernel, fi_erikinkinen_kernel_Stdio, Write,
	      "Write")
DEFINE_INTERFACE(fi_erikinkinen_kernel, fi_erikinkinen_kernel_GlobalNameService,
		 "fi.erikinkinen.kernel.GlobalNameService")
DEFINE_METHOD(fi_erikinkinen_kernel, fi_erikinkinen_kernel_GlobalNameService,
	      RegisterDestination, "RegisterDestination")

void service_handler(uint64_t interface, uint64_t method);
int64_t register_service(char *name)
{
	struct syscall_method_data m = {
		bus_service_fi_erikinkinen_kernel(),
		bus_interface_fi_erikinkinen_kernel_fi_erikinkinen_kernel_GlobalNameService(),
		bus_method_fi_erikinkinen_kernel_fi_erikinkinen_kernel_GlobalNameService_RegisterDestination()
	};
	struct syscall_param name_param = { .type = SYSCALL_PARAM_ARRAY,
					    .size = strlen(name),
					    .array = (void *)name };
	struct syscall_param service_handler_param = {
		.type = SYSCALL_PARAM_PRIMITIVE,
		.array = (void *)&service_handler
	};
	_syscall(SYSCALL_PUSH, &name_param);
	_syscall(SYSCALL_PUSH, &service_handler_param);
	return _syscall(SYSCALL_METHOD, &m);
}

int64_t write(char *str)
{
	struct syscall_method_data m = {
		bus_service_fi_erikinkinen_kernel(),
		bus_interface_fi_erikinkinen_kernel_fi_erikinkinen_kernel_Stdio(),
		bus_method_fi_erikinkinen_kernel_fi_erikinkinen_kernel_Stdio_Write()
	};
	struct syscall_param p = { .type = SYSCALL_PARAM_ARRAY,
				   .size = strlen(str),
				   .array = (void *)str };
	_syscall(SYSCALL_PUSH, &p);
	return _syscall(SYSCALL_METHOD, &m);
}

void service_handler(uint64_t interface, uint64_t method)
{
	char buf[0x100];
	if (interface == 0 && method == 0) {
		struct syscall_param p;
		_syscall(SYSCALL_PEEK, &p);
		if (p.type == SYSCALL_PARAM_ARRAY && p.size < 0x100) {
			p.array = buf;
			_syscall(SYSCALL_POP, &p);
			write("Received: ");
			write(buf);
		} else {
			write("Invalid parameter\n");
		}
	} else {
		write("Invalid method or inteface\n");
	}
	exit(0);
}

int main(void)
{
	int64_t id = register_service("fi.erikinkinen.init");
	char *msg = "Hello from init\n";
	struct syscall_method_data m = { id, 0, 0 };
	struct syscall_param p = { .type = SYSCALL_PARAM_ARRAY,
				   .size = strlen(msg),
				   .array = (void *)msg };
	_syscall(SYSCALL_PUSH, &p);
	_syscall(SYSCALL_METHOD, &m);
	for (;;)
		;
	return 0;
}
