#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/kernel.h>

// Stack and another variable pointing top of the stack.
int stack[1024] = {0};
int ptr = 0;

// Prototype
void print_stack(void);
int check(int x);

// Stack push
SYSCALL_DEFINE1(mypush, int, a)
{
	if (check(a) != 0)
	{
		stack[ptr++] = a;
		printk("Push %d\n", a);
	}
	else // Abort if given value already exists in stack.
		printk("[-] %d is already in stack. aborting...\n", a);
	print_stack();
	return;
}

// Stack pop
SYSCALL_DEFINE0(mypop)
{
	int out;
	if (ptr > 0)
	{
		out = stack[--ptr];
		printk("Pop %d\n", out);
	}
	else // Abort if stack is empty
	{
		printk("[-] Stack is empty. aborting...\n");
		return -1;
	}
	print_stack();
	return out;
	
}

// Print stack from top to bottom
void print_stack(void)
{
	int i;
	printk("<-------------- Stack Top\n");
	for (i = ptr - 1; i >= 0; i--)
	{
		//(value, address, location)
		printk("%d @ %p [Stack data #%d]\n", stack[i], stack[i], i);
	}
	printk("<----------- Stack Bottom\n");

	return;
}

// Check duplicated value
int check(int x)
{
	int i;
	for (i = ptr - 1; i>=0; i--)
	{
		if (x == stack[i])
			return 0;
	}
	return 1;
}
