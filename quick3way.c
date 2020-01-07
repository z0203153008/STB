//参考：https://blog.csdn.net/yjw123456/article/details/90081267

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


//三向字符串快速排序算法
//is_asc:升序还是降序。
static void sort_by_title(int lo, int hi, int d, bool is_asc)
{
	if(hi <= lo)
	{
		return;
	}

	int lt = lo, gt = hi;
	int v = charAt(ps_Livelistdata[lo].name, d);
	int i = lo + 1;

	while(i <= gt)
	{
		int t = charAt(ps_Livelistdata[i].name, d);

		if((is_asc && (t < v)) || (!is_asc && (t > v)))
		{
			live_list_swap(lt, i);
			lt++;
			i++;
		}
		else if((is_asc && (t > v)) || (!is_asc && (t < v)))
		{
			live_list_swap(gt, i);
			gt--;
		}
		else
		{
			i++;
		}
	}

	sort_by_title(lo, lt - 1, d, is_asc);

	if(v >= 0)
	{
		sort_by_title(lt, gt, d + 1, is_asc);
	}

	sort_by_title(gt + 1, hi, d, is_asc);
}

int main(void)
{
  //total为所有需要排序的字符串的总个数
  //ps_Livelistdata[i].name为需要排序的第i个节目名称
  sort_by_title(0, total-1, 0, true)//升序
  return 0;
}
