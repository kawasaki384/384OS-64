#include <stdalign.h>
#include <stdint.h>
#include <stddef.h>
#include "paging.h"

const uint64_t PageSize4K = 4096;
const uint64_t PageSize2M = 512 * PageSize4K;
const uint64_t PageSize1G = 512 * PageSize2M;
/*
一つのページディレクトリにつき512個の2Mibページが設定できるので、
page_directory_count x 1GiBの仮想アドレスがマッピングされることになる。
*/

alignas(4096) uint64_t pml4_table[512];
alignas(4096) uint64_t pdp_table[512];
alignas(4096) uint64_t page_directory[64][512];


void SetupIdentityPagetable() {
    pml4_table[0] = (uint64_t)(&pdp_table[0]) | 0x003;
   //ここの64の部分はC++の
   /*
	page_directory.size();
	を実行した結果が64だったためそうなる。
   

	追記:ちなみにsizeメソッドは縦の要素数を得るための物らしく
	それはCの二次元配列で言うと縦なので64で正解。
	参考:https://atcoder.jp/contests/APG4b/tasks/APG4b_t

	ちなみに縦の要素数は 
	sizeof(page_directory) / sizeof(page_directory[0])
	で求められるらしい。
    */
    for (int i_pdpt = 0; i_pdpt < 64; ++i_pdpt){
        pdp_table[i_pdpt] = (uint64_t)&page_directory[i_pdpt] | 0x003;
        for (int i_pd = 0; i_pd < 512; i_pd++) {
            page_directory[i_pdpt][i_pd] = (i_pdpt * PageSize1G + i_pd * PageSize2M) | 0x083;
        }
    }
    
    SetCR3((uint64_t)&pml4_table[0]);
}

