/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2026 Koji KITAYAMA */
#include <msp432e401y.h>
#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;
  int diff = 0;
  while (n--) {
    diff = *p1++ - *p2++;
    if (diff) break;
  }
  return diff;
}

void SystemInit(void)
{
  /* Turn off power domains that unused peripherals belong to */
  SYSCTL->PCCAN  = 0u;
  SYSCTL->PCEMAC = 0u;
  SYSCTL->PCEPHY = 0u;
  SYSCTL->PCCCM  = 0u;

  /* --- Setup system clock --- */
  /* Start power-up process of the main oscillator */
  SYSCTL->MOSCCTL = SYSCTL_MOSCCTL_OSCRNG;
  while (!(SYSCTL->RIS & SYSCTL_RIS_MOSCPUPRIS)) ; /* Wait for completion */
  SYSCTL->MISC = SYSCTL_MISC_MOSCPUPMIS; /* Clear the completion interrupt status */
  /* Set the main oscillator to PLL reference clock */
  SYSCTL->RSCLKCFG = SYSCTL_RSCLKCFG_PLLSRC_MOSC;
  /* PLL freq. = (MOSC freq. / 10) * 96 = 240MHz */
  SYSCTL->PLLFREQ1 = (4 << SYSCTL_PLLFREQ1_N_S) | (1 << SYSCTL_PLLFREQ1_Q_S);
  SYSCTL->PLLFREQ0 = (96 << SYSCTL_PLLFREQ0_MINT_S) | SYSCTL_PLLFREQ0_PLLPWR;
  /* Set BCHT=6, BCE=0, WS=5 for 120MHz system clock */
  SYSCTL->MEMTIM0 = SYSCTL_MEMTIM0_EBCHT_3_5 | (5 << SYSCTL_MEMTIM0_EWS_S) |
    SYSCTL_MEMTIM0_FBCHT_3_5 | (5 << SYSCTL_MEMTIM0_FWS_S) | SYSCTL_MEMTIM0_MB1;
  /* Wait for completion of PLL power-up process */
  while (!(SYSCTL->RIS & SYSCTL_RIS_PLLLRIS)) ;
  SYSCTL->MISC = SYSCTL_MISC_PLLLMIS; /* Clear the completion interrupt status */
  /* Switch the system clock to PLL/4 */
  SYSCTL->RSCLKCFG = SYSCTL_RSCLKCFG_MEMTIMU | SYSCTL_RSCLKCFG_ACG |
         SYSCTL_RSCLKCFG_USEPLL | SYSCTL_RSCLKCFG_PLLSRC_MOSC | (1 << SYSCTL_RSCLKCFG_PSYSDIV_S);
}
