# CONTRIBUTING

## aliases for zsh-autoenv

### `.autoenv_enter.zsh`

```shell
# for Debian 13 or some other Linux distributions
alias writehex='
cd /opt/microchip/mplabx/v6.20/mplab_platform/mplab_ipe
sudo sh ipecmd.sh \
--% -P12F675 -TPPK3 -M \
-F"/home/asumo/Remote/NormalTrueBypass_PIC/out/PIC12F675/default.hex"
'

# # for Windows 11
# alias writehex='
# & "C:\Program Files\Microchip\MPLABX\v6.20\mplab_platform\mplab_ipe\ipecmd.exe" --% -P12F675 -TPPK3 -M -F"C:\Users\asumo\MPLABProjects\Lchica\out\Lchica\default.hex"
# '
```

### `.autoenv_leave.zsh`

```shell
# for Debian 13 or some other Linux distributions
unalias writehex

# # for Windows 11
# unalias writehex
```
