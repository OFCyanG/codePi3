script_node_red_services="[Unit]\nDescription=Node-RED\nAfter=syslog.target network.target\n\n[Service]\nExecStart=`which 'node-red'`-pi --max-old-space-size=128 -v\nRestart=on-failure\nKillSignal=SIGINT\nSyslogIdentifier=node-red\nStandardOutput=syslog\nWorkingDirectory=/home/`echo $USER`/\nUser=`echo $USER`\n\n[Install]\nWantedBy=multi-user.target"

echo -e $script_node_red_services > ./node-red.service
sudo cp -f ./node-red.service /etc/systemd/system/node-red.service

sudo systemctl daemon-reload
sudo systemctl enable node-red.service
sudo systemctl start node-red.service
