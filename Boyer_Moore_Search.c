
//获取字符串索引处的字符
static int charAt(char *s, int d)
{
	if(d < strlen(s))
	{
		return toupper(s[d]);
	}
	else
	{
		return -1;
	}
}

static void getRight(char* pat, int* right)
{
	int i = 0;

	for (i = 0; i < 256; i++)
	{
		right[i] = -1;
	}

	for (i = 0; i < strlen(pat); i++)
	{
		right[charAt(pat, i)] = i;
	}
}

static int BoyerMooreSearch(char* txt, char* pat, int* right)
{
	int M = strlen(txt);
	int N = strlen(pat);
	int skip = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i <= M - N; i += skip)
	{
		skip = 0;

		for (j = N - 1; j >= 0; j--)
		{
			if (toupper(charAt(pat, j)) != toupper(charAt(txt, i + j)))
			{
				skip = j - right[charAt(txt, i + j)];

				if (skip < 1)
				{
					skip = 1;
				}

				break;
			}
		}

		if (skip == 0)
			return i;
	}

	return -1;
}

int main(void)
{
	char *search = "example";
	char *test_sample = "here is a simple example";
	int right[256] = {0};/*用于匹配*/
	bool match = false;
  
	for(i = 0; i < g_vodlist_total; i++)
	{
		getRight(search,right);

		if (BoyerMooreSearch(test_sample, search, right) != -1)
		{
			match = true;
		}
	}
}
