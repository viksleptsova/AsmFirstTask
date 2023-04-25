#include <stdio.h>
#include <stdlib.h>


int strstr_index(const char* _str, const char* _substr)
{
	__asm {
		mov		esi, _str
		mov		edi, _substr				
		mov		eax, -1; int i = -1;
	STR_CYCLE:; STR_CYCLE:
		inc		eax; while (str[++i] != 0)
			cmp		byte ptr[esi + eax], 0; {
			jz		NOT_FOUND;
			xor ecx, ecx;     int j = 0;
		SUBSTR_CYCLE:;     char dh;
			mov		dh, [edi + ecx];     while ((dh = substr[j]) != 0)
				cmp		dh, 0; {
				jz		FOUND;
				;
				add		eax, ecx;         char dl = str[i + j];
				mov		dl, [esi + eax];
				sub		eax, ecx;
				;
				cmp		dl, dh;         if (dl != dh) goto STR_CYCLE;
				jne		STR_CYCLE;
				inc		ecx;         ++j;
				jmp		SUBSTR_CYCLE;     }
			;     return i;
		NOT_FOUND:; }
			mov		eax, -1; return -1;

		FOUND: 
	}
}

int main()
{
	const char welcome[] = "input string:";
	const char question[] = "input substring whose index you want to find in the first one:";
	const char substr_not_found[] = "substring was not found. try again.";
	const char printf_params[] = "index: %d\n";

	const int MAX_SIZE = 100;
	char* _str = nullptr, * _substr = nullptr;

	__asm {
		// _str = (char*) malloc(MAX_SIZE);
		push	dword ptr MAX_SIZE
		call	malloc
		mov[_str], eax

		// _substr = (char*) malloc(MAX_SIZE);
		call	malloc
		mov[_substr], eax

		// puts(welcome);
		lea		eax, welcome
		push	eax
		call	puts

		// gets_s(_str, MAX_SIZE);
		mov		eax, _str
		mov[esp], eax
		call	gets_s

		CYCLE :
		// puts(question);
		lea		eax, question
			mov[esp], eax
			call	puts

			// gets_s(_substr, MAX_SIZE);
			mov		eax, MAX_SIZE
			mov[esp + 4], eax
			mov		eax, _substr
			mov[esp], eax
			call	gets_s

			// strstr_index(_str, _substr);
			mov		eax, _substr
			mov[esp + 4], eax
			mov		eax, _str
			mov[esp], eax
			call	strstr_index

			cmp		eax, -1
			jne		END

			// puts(substr_not_found);
			lea		eax, substr_not_found
			mov[esp], eax
			call	puts

			jmp		CYCLE
			END :
		// printf_s(printf_params, eax);
		mov[esp + 4], eax
			lea		eax, printf_params
			mov[esp], eax
			call	printf_s

			// free(_str);
			mov		eax, _str
			mov[esp], eax
			call	free

			// free(_substr);
			mov		eax, _substr
			mov[esp], eax
			call	free

			add		esp, 8
			xor eax, eax
	}
}
