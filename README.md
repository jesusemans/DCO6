# DCO6

Sintetizador VST3 polifónico de 6 voces. El sonido está inspirado en el Juno-60 pero sin copiar la estética ni la interfaz de Roland. Hecho desde cero en JUCE/C++.

**Esto está en beta. Va a ir mejorando con el tiempo.**

---

## Descargar (sin compilar)

Si solo querés usar el plugin, descargá los binarios de la sección [Releases](../../releases/latest):

- `DCO6 v1.1.vst3` - para cargar en cualquier DAW que soporte VST3 (Ableton, Reaper, FL Studio, etc.)
- `DCO6 v1.1.exe` - standalone, corre sin DAW

Para el VST3: copiá la carpeta `DCO6 v1.1.vst3` a `C:\Program Files\Common Files\VST3\` y escaneá los plugins desde tu DAW.

---

## Características

- 6 voces polifónicas con polyBLEP (anti-aliasing)
- Filtro ladder de 4 polos tipo Moog (Huovilainen)
- Coro BBD estéreo (Modo I y II)
- LFO con delay, delay ping-pong estéreo, reverb
- ADSR de amplitud y de filtro independientes
- EQ de 3 bandas (low shelf 200Hz, peak 1kHz, high shelf 5kHz)
- Analizador de espectro FFT en tiempo real
- Skin clara y skin neon (se cambia desde el plugin)
- 8 presets de pad incluidos
- Mapeo MIDI completo para Arturia KeyLab 61

### Mapeo MIDI - Arturia KeyLab 61

Knobs:

| CC | Parámetro |
|----|-----------|
| 74 | VCF Cutoff |
| 71 | VCF Resonance |
| 76 | VCF Env Amount |
| 77 | DCO PWM |
| 78 | LFO Rate |
| 84 | LFO Delay |
| 86 | Delay Mix |
| 87 | Reverb Mix |
| 88 | DCO LFO Pitch |

Faders:

| CC | Parámetro |
|----|-----------|
| 73 | Filter Env Attack |
| 75 | Filter Env Decay |
| 79 | Filter Env Sustain |
| 72 | Filter Env Release |
| 80 | Amp Env Attack |
| 81 | Amp Env Decay |
| 82 | Amp Env Sustain |
| 83 | Amp Env Release |
| 85 | Volume |

---

## Compilar desde el código fuente

### Requisitos

- Windows 10/11 x64
- [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) con "Desarrollo de escritorio con C++"
- [CMake 3.22+](https://cmake.org/download/)
- [Git](https://git-scm.com/)

JUCE se descarga solo la primera vez que compilás (via CMake FetchContent, necesita internet).

### Pasos

```bat
git clone https://github.com/jesusemans/DCO6.git
cd DCO6
build.bat
```

El script configura CMake, compila en Release e instala el VST3 automáticamente en `C:\Program Files\Common Files\VST3\`. La primera compilación tarda entre 5 y 10 minutos.

### Versionar manualmente

Para generar una nueva versión (ej. v1.2), cambiá estas dos líneas en `CMakeLists.txt`:

```cmake
set(DCO6_VERSION "1.2")
set(DCO6_CODE    "D612")
```

Y en `build.bat`:

```bat
set VERSION=1.2
```

El código de 4 chars tiene que ser único por versión para que los DAWs las distingan como plugins separados.

---

---

# DCO6 (English)

6-voice polyphonic VST3 synthesizer. The sound draws inspiration from the Juno-60 but the UI and workflow are original. Built from scratch in JUCE/C++.

**This is beta software. It will keep improving.**

---

## Download (no build required)

Grab the binaries from [Releases](../../releases/latest):

- `DCO6 v1.1.vst3` - load in any VST3-compatible DAW (Ableton, Reaper, FL Studio, etc.)
- `DCO6 v1.1.exe` - standalone, runs without a DAW

For the VST3: copy the `DCO6 v1.1.vst3` folder to `C:\Program Files\Common Files\VST3\` and rescan plugins in your DAW.

---

## Features

- 6-voice polyphony with polyBLEP anti-aliasing
- 4-pole Moog-style ladder filter (Huovilainen model)
- BBD stereo chorus (Mode I and II)
- LFO with delay, stereo ping-pong delay, reverb
- Independent amplitude and filter ADSRs
- 3-band EQ (low shelf 200Hz, peak 1kHz, high shelf 5kHz)
- Real-time FFT spectrum analyzer
- Light and neon skin (switchable inside the plugin)
- 8 pad presets included
- Full MIDI CC mapping for Arturia KeyLab 61

### MIDI Mapping - Arturia KeyLab 61

Knobs:

| CC | Parameter |
|----|-----------|
| 74 | VCF Cutoff |
| 71 | VCF Resonance |
| 76 | VCF Env Amount |
| 77 | DCO PWM |
| 78 | LFO Rate |
| 84 | LFO Delay |
| 86 | Delay Mix |
| 87 | Reverb Mix |
| 88 | DCO LFO Pitch |

Faders:

| CC | Parameter |
|----|-----------|
| 73 | Filter Env Attack |
| 75 | Filter Env Decay |
| 79 | Filter Env Sustain |
| 72 | Filter Env Release |
| 80 | Amp Env Attack |
| 81 | Amp Env Decay |
| 82 | Amp Env Sustain |
| 83 | Amp Env Release |
| 85 | Volume |

---

## Building from source

### Requirements

- Windows 10/11 x64
- [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) with "Desktop development with C++"
- [CMake 3.22+](https://cmake.org/download/)
- [Git](https://git-scm.com/)

JUCE is downloaded automatically on first build (via CMake FetchContent, needs internet).

### Steps

```bat
git clone https://github.com/jesusemans/DCO6.git
cd DCO6
build.bat
```

The script configures CMake, builds in Release mode, and auto-installs the VST3 to `C:\Program Files\Common Files\VST3\`. First build takes 5-10 minutes.

### Versioning

To release a new version (e.g. v1.2), change these two lines in `CMakeLists.txt`:

```cmake
set(DCO6_VERSION "1.2")
set(DCO6_CODE    "D612")
```

And in `build.bat`:

```bat
set VERSION=1.2
```

The 4-char plugin code must be unique per version so DAWs treat them as separate plugins.

---

## License

This project is provided as-is. No warranty. Use at your own risk.
