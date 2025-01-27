# Spectral Toolbox

## Сборка

Необходимо поставить qt6.5+, gtest

Собрать можно командой

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 8
```

Запустить приложение:

```bash
./build/SpectralToolbox
```

Запустить тесты

```bash
./build/test/test_spectral_toolbox
# или
ctest --test-dir ./build
```
