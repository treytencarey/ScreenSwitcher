# ScreenSwitcher

ScreenSwitcher is a portable, easy-to-use application that allows one or more PCs to access hotkeys of another PC (via internet connection).

  - Configure your own hotkey combos.
  - Hotkeys send to connected PC, pressing corresponding hotkey.
  - Used primarily for OBS Studio, allowing people who stream together to both change OBS scenes and receieve audio feedback of scene changes.

# New Features!

  - Play any sound you want when a scene becomes active or inactive.
  - Audio feedback is purposefully delayed as to not interfere with your stream/recordings.


You can also:
  - Connect on any internet connection (local or remote).
  - Enable sounds to play when you change scenes yourself, or only when someone else changes the scene for you.

Configuration is easy, and examples are already provided in the config.ini files.

> // Client uses these to tell who to connect to
> IPV4=192.168.0.23
> PORT=2322
> // Use keycodes from: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
> // Hotkeys use '+' delimiters, so 11+31 means "press Shift and 1 at the same time". Give this action any name:
> Game Queue=11+31
> Game - Trey=11+32
> Game - Nate=11+33
> // syntax: [sceneName]soundFile -> Plays the sound if the scene becomes active
> //         [all]soundFile -> Plays the sound if the scene changes.
> //         [sceneName]soundFile[all]otherSoundFile -> Plays soundFile if the scene becomes active, otherwise plays otherSoundFile if any other scene becomes active.
> //         [!sceneName]soundFile -> Plays soundFile if the scene becomes inactive.
> //         [sceneName]soundFile[!sceneName]otherSoundFile[all]anotherSoundFile -> Plays soundFile if the scene becomes active, plays otherSoundFile if it becomes inactive, and plays anotherSoundFile if any other scene becomes active.
> PLAYSOUND=[Game - Nate]connect.mp3[!Game - Nate]disconnect.mp3