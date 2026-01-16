# hardbird_attack
PoC showcasing the SEPROM hardbird attack on Apple A7 SoCs by getting TZ r/w.

## Usage
Compile the hardbird_attack Pongo module:
```
make all
```

Use my PongoOS fork (linked as a gitmodule) as it has a fix for non A10 devices where it uses a 4KB offset for calculating the mailbox register ptr instead of 16KB that stock Pongo uses.
```
git submodule update --init --recursive
cd PongoOS
make all
```

Boot Pongo with checkra1n-1337:
```
checkra1n-1337 -cpk build/Pongo.bin
cd scripts
make
./pongoterm
```
Then finally, send and run the hardbird_attack Pongo module within pongoterm
```
/send <path-to-module>
modload hardbird_attack
hardbird_attack
```

## Credits
[Proteas](https://x.com/ProteasWang) - discovered the hardbird vulnerability

[checkra1n](https://x.com/intent/follow?screen_name=checkra1n) - developed PongoOS, etc

[TheRealClarity](https://x.com/imnotclarity) - helped me get started, helped me understand the vulnerability significantly and informed me about the mailbox register ptr issue + fix

[Alfie](https://x.com/alfiecg_dev) - also helped me understand the vulnerability
