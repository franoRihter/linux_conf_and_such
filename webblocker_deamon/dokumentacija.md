#Web daemon

##file vjezbe ima funkciju deamonize();
###on stvara fork koji djetetu dava sve povlastive
###to je potrebno zato što se na taj način odvaja porgram od terminala također dobija novi porces id
###također funkcija blokira pristup stdinout stdoutput. To radi tako da upisiva stvati u /null file na linuxu koji ništa ne vraća.

##ostatak koda je evidentan

##primjena daemona koristeći systemd
### kopiro sam compilani kod (koristio gcc) u /usr/locak/bin/
```
sudo cp moj_daemon /usr/local/bin/
sudo chmod +x /usr/local/bin/moj_daemon

```
##stvorio sam Unit file u /etc/systemd/system/moj_daemon.service

##pokrenuo daemon
```
sudo systemctl daemon-reexec #reloadanje systemd
sudo sstemctl daemon-reload #isto valjda
sudo systemctl enable moj_daemon.service
sudo systemctl start moj_daemon.service
```

##provjera logova
```
sudo systemctl status moj_daemon
journalctl -u moj_daemon
```


