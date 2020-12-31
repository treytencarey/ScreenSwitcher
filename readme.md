# ScreenSwitcher

ScreenSwitcher is a portable, easy-to-use application that allows one or more PCs to access hotkeys of another PC (via internet connection).

  - Configure your own hotkey combos.
  - Hotkeys send to connected PC, pressing corresponding hotkey.
  - Used primarily for **OBS Studio**, allowing people who stream together to both change OBS scenes and receieve audio feedback of scene changes.
  - Play any sound you want when a scene becomes active or inactive.

See this program in action live by following our Twitch channel at [twitch.tv/4FeetApart](https://www.twitch.tv/4feetapart).

# How to Run

  - Update your sounds and config file in the Client/Client and Server/Server directory.
  - Run the Server/Run-Release.bat on your streamer PC and the Client/Run-Release.bat on your other PC.

# New Features!

  - Play sounds after a configured delay (TRANSITIONPOINT in config.ini).
  - Create new sound combinations by telling what sound should play when going from what scene to what scene (PLAYSOUND in config.ini).
  - Choose whether or not your own scene switches (not triggered by someone else) should also trigger sounds to play.
  - You can now assign multiple hotkeys to the same scene. When the client(s) switch a scene, whichever hotkey for the same scene comes first is triggered.

You can also:
  - Connect on any internet connection (local or remote).
  - Enable sounds to play when you change scenes yourself, or only when someone else changes the scene for you.

Configuration is easy, and examples are already provided in the config.ini files.

> // Client uses these to tell who to connect to.\
> IPV4=192.168.0.23\
> PORT=2322\
> // Use keycodes from: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes (leave as hex value, so 0x31 from the link is 31 below).\
> // Hotkeys use '+' delimiters, so 11+31 means "press Shift and 1 at the same time". Give this action any name:\
> Game Queue=11+31\
> // Notice below that we have two of the same scene (Game - Trey).
> //   Using the same scene twice allows us to use multiple hotkeys for a scene switch.
> //   When someone else triggers a scene switch, whichever hotkey assigned FIRST is the one that is triggered on the host PC (in this case, DD which maps to key '[').
> Game - Trey=DD\
> Game - Trey=11+32\
> Game - Nate=11+33\
> // syntax: [sceneName]soundFile -> Plays the sound if the scene becomes active\
> //         [all]soundFile -> Plays the sound if the scene changes.\
> //         [sceneName]soundFile[all]otherSoundFile -> Plays soundFile if the scene becomes active, otherwise plays otherSoundFile if any other scene becomes active.\
> //         [sceneName->all]soundFile -> Plays soundFile if the scene becomes inactive.\
> //         [sceneName]soundFile[sceneName->all]otherSoundFile[all]anotherSoundFile -> Plays soundFile if the scene becomes active, plays otherSoundFile if it becomes inactive, and plays anotherSoundFile if any other scene becomes active.\
> // Plays connect.mp3 when the scene "Game - Nate" is transitioned to from all other scenes. Plays disconnect.mp3 when the scene "Game - Nate" is transitioned to any other scene.\
> PLAYSOUND=[all->Game - Nate]connect.mp3[Game - Nate->all]disconnect.mp3\
> // Choose whether or not sounds play when you yourself change the scene (opposed to just someone else). Set to 1 for True, 0 for False.\
> SELFSOUNDS=1\
> // In OBS, this is the scene transition time. Delays sounds until the transition is complete.\
> TRANSITIONPOINT=800