//header files
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

//define
int Cache;// =  8*1024; //Cache size
int E;//= 1; //number of lines in each cache set
int B;//= 64; //number of bytes in data block of each cache line
int S;//= Cache/(E*B); //number of sets

//returns log to base 2
int mylog(int num)
{
	return (log(num)/log(2));
}

int myatoi(char string[])
{
	int num, i, j;
	num = 0;
	for(i=0; string[i]!='\0' ; i++)
	{
		num *= 10;
		j = string[i]-'0';
		num += j;
	}
	return num;
}

//main funtion
void main(int argc, char* argv[])
{
	//structure of each cache line
	typedef struct l
	{
	        int count;
        	int valid; //valid bit
	        int tag; //tag bits
	}cacheline;

	//structure of each set
	typedef struct s
	{
        	cacheline line[E]; //array of sets
	}set;
	Cache = myatoi(argv[1]);
	E = myatoi(argv[2]);
	B = myatoi(argv[3]);
	S = Cache/(E*B);
	long int address; //input address
	long int block, index, mytag; //fields in each address
	set cache[S]; //array of sets ie., cache

	int i, j, flag = 0, c;
	float hit, miss;
	float hit_rate, miss_rate;
	int b, s, m, t; //number of bits in block field, set index field and tag field

	hit = miss = 0;
	m = 48;
	b = mylog(B);
	s = mylog(S);
	t = m - (b + s);
	b = pow(2,b);
	s = pow(2,s);

//initialise all cache fields to zero
	for(i=0 ; i<S ; i++)
	{
		for(j=0; j<E; j++)
		{
			cache[i].line[j].valid = 0;
			cache[i].line[j].tag = 0;
			cache[i].line[j].count = 0;
		}
	}
//	scanf("%lx", &address);
	while(scanf("%lx",&address) != EOF)
	{
		flag = 0;
		block = address % b; //get the block bits
		address /= b; //get the remaining address
		index = address % s; //get the set index bits
		address /= s; //get the remaining address
		mytag = address;

		for(i=0 ; i<E ; i++)
		{
			if(cache[index].line[i].valid == 1 && cache[index].line[i].tag == mytag)
			{
				hit++;
				cache[index].line[i].count++;
				flag = 1;
				break;
			}
		}
		if(!flag)
		{
			miss++;
			//j is the index to replace the cache line
			for(i=0 ; i<E ; i++)
			{
				if(cache[index].line[i].valid == 0)
				{
					j = i;
					flag = 1;
					break;
				}
			}
			if(!flag)
			{
				c = cache[index].line[0].count;
				for(i=1; i<E ; i++)
				{
					if(cache[index].line[i].count < c)
					{
						c = cache[index].line[i].count;
						j = i;
					}
				}
			}
			cache[index].line[j].valid = 1;
			cache[index].line[j].tag = mytag;
		}
//		scanf("%lx",&address);
	}
	miss_rate = miss/(miss+hit) * 100;
	hit_rate = hit/(miss+hit) *100;
	printf("\n Miss rate = %f \n Hit rate = %f \n", miss_rate, hit_rate);
}
