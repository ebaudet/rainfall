#include <string.h>

class N {  // ebp+0x8
public :
	char annotation[100];  // ebp+0x8 + 0x4 | size : 0x64 (0x68 - 0x4)
	int value;  // ebp+0x8 + 0x68

	N(int val) : value(val) {}  // 0x080486f6
	void	setAnnotation(char *str) {  // 0x0804870e
		int len = strlen(str);
		memcpy(annotation, str, len);
	}
	int		operator+(N const &rhs) {  // 0x0804873a
		return value + rhs.value;
	}
	int		operator-(N const &rhs) {  // 0x0804874e
		return value - rhs.value;
	}
};

int		main(int ac, char **av)
{
	if (ac <= 1)
		_exit(1);
	N *a = new N(5);  // esp+0x1c
	N *b = new N(6);  // esp+0x18

	N *c = a;  // esp+0x14
	N *d = b;  // esp+0x10

	c->setAnnotation(av[1]);
	return (*d + *c);
}
