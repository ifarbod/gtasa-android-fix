# CTNorth <img src="http://meowy.cf/x/170322_C0v.png" width="32" height="32" alt="CTNorth" />

[![GitHub issues](https://img.shields.io/github/issues/ctnorth/ctnorth.svg)](https://github.com/ctnorth/ctnorth/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/ctnorth/ctnorth.svg)](https://github.com/ctnorth/ctnorth/pulls)
[![GitHub release](https://img.shields.io/github/release/ctnorth/ctnorth.svg)](https://github.com/ctnorth/ctnorth/releases/latest)
[![GitHub contributors](https://img.shields.io/github/contributors/ctnorth/ctnorth.svg)](https://github.com/ctnorth/ctnorth/graphs/contributors)
[![License](https://img.shields.io/github/license/ctnorth/ctnorth.svg)](https://github.com/ctnorth/ctnorth/blob/master/LICENSE.md)
[![Total downloads](https://img.shields.io/github/downloads/ctnorth/ctnorth/total.svg)](https://github.com/ctnorth/ctnorth/releases)

CTNorth is a framework and an online multiplayer modification for Rockstar North's Grand Theft Auto III series.

Primarily focused on GTA: San Andreas; As this game doesn't have a _built-in_ network play functionality, many attempts have been made since its PC release and only some of them succeeded. CTNorth has a goal of being unique by implementation of artifical intelligence synchronization which will add a unique co-operative based gameplay type, and being _completely_ customizable, ranging from custom maps, weapons, vehicles to scripts that change the entire game logic.

-------------------------------------------------
## Build & Test Status
|                    | Release x86 | Release x64 |
|:-------------------|------------:|------------:|
| **AppVeyor** | [![Build status](https://ci.appveyor.com/api/projects/status/8sm6rvql5e9trkar/branch/master?svg=true)](https://ci.appveyor.com/project/ctnorth/ctnorth/branch/master) |  |
| **Travis CI**  |             |             |

## Community
[![Discord guild](https://img.shields.io/badge/chat-on%20Discord-7085D4.svg)](https://discord.gg/Tx2mHuc)

## Key Features (Planned)
* A.I./Co-op
  - The most important of them all! Bored of playing in an empty map with no pedestrians? Or maybe you want to play some missions with your friend(s)? or even trying to survive hordes of enemies? Thanks to our custom AI system, A true co-op experience is now possible in the world of San Andreas. Something never seen before in similar mods!
* Dedicated servers
  - The old school way of finding that perfect game. Set up your filters and you're ready to go.
  - Custom rules? No problem. You can fine-tune the game logic using the power of our scripting system to achieve your perfect game mode. Wheather it's an advanced roleplaying or a classic deathmatch mode.
* Peer to peer
  - Can't be bothered to host a server? Well, with our P2P networking layer you don't have to worry about hosting a server at all. Either jump in a quick match or create a lobby with your friends (and foes?), change the game's rules, then play the game as you wish!
* Server content streaming
  - You want to stream modifications or content from the server? No problem. Just hook up your preferred CDN system or use the integrated HTTP server for streaming content in our dedicated server.
* Limit adjuster
  - Do you think the vanilla map of San Andreas is not enough for you? You want to add some content to San Andreas but GTA limits prevent you to do so? Whatever you want to do, from adding a dozens of weapons, to increasing the draw distance, our integrated limit adjuster is there for you!
* 2 scripting languages (AngelScript and C#)
  - Script your custom missions, mods and game logic in either C#, AngelScript or even, both of them!
* Client-side scripting
  - You may need to do something hacky on a specific player, with client-side scripting, It's possible to do so! Your script will be downloaded to the target client and it will be executed. Communication between server-side scripts and client-side ones is also possible.
* Integrated CEF (Chromium Embedded Framework) for rich HTML5+CSS3 powered user interfaces
  - Did you find the stock user interface boring? Using CEF, It's possible to bring powerful modern web technologies to CTNorth! Use your web developing skills to create that decent GUI!
* Auto-updater
  - No need of manually updating your client anymore! New updates download silently as you play or before launching the client! With delta compression, Most of the updates are as small as 3 MB.

[![Join the chat at https://gitter.im/sanandreasonline/sao](https://badges.gitter.im/sanandreasonline/sao.svg)](https://gitter.im/sanandreasonline/sao?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## License

[MIT](LICENSE.md) © 2015-2017 CTNorth Team

[Click here for a TL;DR version of the license](https://tldrlegal.com/l/mit)

CTNorth is in no way affiliated with or endorsed by Rockstar North and/or Take-Two Interactive.
All trademarks used are property of their respective owners.
