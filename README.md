# DigitalGuitarEffect

## Compilation and run:
(in root directory)

```
cmake -B build
cmake --build build --config Release
```
### Linux & MacOS:
Run app:
```
python3 src/python/main.py
```
Run tests:
```
./bin/app_test
```
### Windows:
Run app:
```
py src/python/main.py
```
Run tests:
```
.\bin\Release\app_test.exe
```
## Requirements:
- cmake
- python 3.11+

    *packages:*
    - PyQt6
    - matplotlib
- Visual Studio 17 2022 (Windows) / G++ (Linux)
- Compiled portaudio library

------------
### Portaudio Compilation:
#### Windows:
- clone portaudio repository: https://github.com/PortAudio/portaudio.git
```
git clone https://github.com/PortAudio/portaudio.git
```
- change directory to portaudio
```
cd portaudio
```
- create build folder
```
mkdir build
```
- change directory to build:
```
cd build
```
- generate visual studio project using asio:
```
cmake -G "Visual Studio 17 2022" -A x64 -DPA_USE_ASIO=OFF ..
```
or with asio:
```
cmake -G "Visual Studio 17 2022" -A x64 -DPA_USE_ASIO=ON ..
```
- run portaudio/build/PortAudio.sln file
- go to Solution Explorer and open PortAudio properties (configuration: ***Release*** Platform: ***x64***)
- in ***General*** set 'Configuration Type' to '.lib'
- in ***Advanced*** set 'Target File Extension' to '.lib'
- in ***C/C++*** -> ***Code Generation*** set 'Runtime Library' to '/MD'
- build Solution (Release)
- Copy folder portaudio/include and lib file portaudio/build/Release/portaudio.lib to DigitalGuitarEffect/lib/portaudio

#### Linux:
- install alsa:

(Ubuntu):
```
sudo apt install libasound2-dev
```

(Fedora):
```
sudo dnf install alsa-lib-devel
```
- clone portaudio repository: https://github.com/PortAudio/portaudio.git
```
git clone https://github.com/PortAudio/portaudio.git
```
- change directory to portaudio
```
cd portaudio
```
- set -fPIC flag:
```
export CFLAGS="-fPIC"
```
- compile project:
```
./configure & make
```
- Copy folder portaudio/include and lib file portaudio/build/libportaudio.a to DigitalGuitarEffect/lib/portaudio

#### MacOS:
- create lib directory
```
mkdir lib && cd lib
```
- clone portaudio repository: https://github.com/PortAudio/portaudio.git
```
git clone https://github.com/PortAudio/portaudio.git
```
- change directory to portaudio
```
cd portaudio
```
- create build folder
```
mkdir build && cd build
```
- compile project:
```
cmake .. && make
```