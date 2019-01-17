![travisci](https://www.travis-ci.org/number317/ssh-tool.svg?branch=master)

# Intro

This is a small tool to manage ssh server info in command line. Write down the servers' info in the config file so that you don't need to remember the servers' ip, password or port.

# Dependency

* libconfig: parse cfg
* ncurse: draw gui
* sshpass: take password form config file

# shortcut

* <kbd>e</kbd>: edit config with `$EDITOR`
* <kbd>j</kbd>: move down
* <kbd>k</kbd>: move up
* <kbd>r</kbd>: reload
* <kbd>s</kdb>: toggle password
* <kbd>q</kbd>: exit
* <kbd>G</kbd>: move to last
* <kbd>Enter</kbd>: connect

# config

The default config file is in `$HOME/.config/ssh-tool/hosts.cfg`, you can also change the file path by `ssh-tool /path/to/config_file`.

config template:

```cfg
header=["hostname", "ip", "port", "username", "password", "intro"];
seperation_char="━"
seperation_length=90;
hosts=(
        {
        hostname="test2";
        ip="111.111.111.111";
        port="22";
        username="root";
        password="admin";
        comment="test1";
        },
        {
        hostname="test2";
        ip="222.222.222.222";
        port="22";
        username="root";
        password="admin";
        comment="test2";
        }
      );
```

The config file should have `hostname`, `ip`, `port`, `user`, `password`, `intro` six part.

# screenshot

![hide](./img/hide.jpg)

![show](./img/show.jpg)
