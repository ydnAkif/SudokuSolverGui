# 🚀 GitHub Kurulum Talimatları

## GitHub Private Repository'sine Push Etmek İçin Adımlar

### 1. GitHub'da Yeni Repository Oluştur

```
1. GitHub.com'a gir (https://github.com/login)
2. "+" → "New repository" seç
3. Repository adı: SudokuSolverGui
4. Description: "Professional Sudoku Solver GUI with Qt6"
5. Private seç (✓ Private)
6. "Create repository" tıkla
```

### 2. SSH Key Oluştur (İlk Kez Ise)

```bash
ssh-keygen -t ed25519 -C "akif@example.com"
# Enter file location: ~/.ssh/id_github (Enter ile geç)
# Enter passphrase: (boş bırak veya şifre gir)
```

### 3. GitHub'a SSH Key Ekle

```bash
# Key'i kopyala
cat ~/.ssh/id_github.pub

# Tarayıcıda GitHub Settings → SSH and GPG keys → New SSH key
# Title: MacBook
# Key: Yukarıda kopyalanan içeriği yapıştır
# "Add SSH key" tıkla
```

### 4. SSH Config'i Ayarla (Opsiyonel)

```bash
# ~/.ssh/config dosyası oluştur:
Host github.com
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_github
    AddKeysToAgent yes
```

### 5. Git Remote URL'ini Ayarla

```bash
cd /Users/akifaydin/CLionProjects/SudokuSolverGui

# GitHub'dan kopyalanan SSH URL'ini kullan:
git remote add origin git@github.com:AkifAydin/SudokuSolverGui.git
# VEYA HTTPS URL (token gerekli):
git remote add origin https://github.com/AkifAydin/SudokuSolverGui.git
```

### 6. Push Etmeden Önce Dalı Ayarla

```bash
# Main branch'ı default yap
git branch -M main

# Upstream'i ayarla
git remote -v  # Kontrol et
```

### 7. GitHub'a Push Et

```bash
git push -u origin main
```

### 8. Hata Varsa - Personal Access Token Kullan

```bash
# GitHub Settings → Developer settings → Personal access tokens → Tokens (classic)
# "New token (classic)" seç
# Scopes: repo, read:user, user:email
# Token'ı kopyala ve aşağıdaki gibi kullan:

git remote set-url origin https://USERNAME:GITHUB_TOKEN@github.com/AkifAydin/SudokuSolverGui.git
git push -u origin main
```

---

## Kurulumdan Sonra

### Repository Gizlilik Ayarları

```
1. GitHub repository sayfasına git
2. Settings → Private (zaten seçili)
3. Collaborators → Add people (opsiyonel)
```

### İlk Push Başarılı Oldu mu?

```bash
# GitHub'da görünüyor mu kontrol et
# https://github.com/AkifAydin/SudokuSolverGui

# Sonraki push'lar daha kolay:
git push origin main
```

### Dosyaları Güncelle ve Push Et

```bash
# Değişiklikler yap
echo "Updated" >> README.md

# Stage ve commit
git add .
git commit -m "Update: Description of changes"

# Push
git push origin main
```

---

## Sorun Giderme

### SSH bağlantısı başarısız

```bash
# SSH debug mode:
ssh -vvv git@github.com

# Alternatif: HTTPS kullan
git remote set-url origin https://github.com/AkifAydin/SudokuSolverGui.git
```

### Authentication başarısız

```bash
# Credentials cache'ini temizle
git credential-osxkeychain erase
host=github.com
protocol=https

# VEYA SSH key'i SSH agent'a ekle
ssh-add ~/.ssh/id_github
```

### "Repository not found"

```bash
# Repository adını kontrol et (büyük/küçük harf duyarlı)
# Private repository'ye access var mı kontrol et
git remote -v  # URL kontrol et
```

---

## ✅ Başarılı Kurulum Kontrol Listesi

- [ ] GitHub'da private repository oluşturdum
- [ ] SSH key'i oluşturdum
- [ ] GitHub'a SSH key'i ekledim
- [ ] Git remote URL'ini ayarladım
- [ ] Main branch'ı ayarladım
- [ ] `git push origin main` başarılı
- [ ] GitHub'da dosyaları görüyorum

---

## 📁 Push Edilen Dosyalar

```
SudokuSolverGui/
├── .gitignore                     # Git ignore patterns
├── LICENSE                        # MIT License
├── README.md                      # Complete documentation
├── CMakeLists.txt                 # Build configuration
├── PROJECT_ANALYSIS.md            # Detailed analysis
├── IMPROVEMENTS_APPLIED.md        # Optimization report
├── BUILD_COMPLETION_REPORT.md     # Build status
│
├── include/
│   ├── MainWindow.hpp
│   ├── SudokuGrid.hpp
│   ├── SudokuCell.hpp
│   └── Sudoku.hpp
│
├── src/
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── SudokuGrid.cpp
│   ├── SudokuCell.cpp
│   └── Sudoku.cpp
│
├── ui/
│   └── MainWindow.ui
│
├── resources/
│   └── resources.qrc
│
└── tests/
    └── test_sudoku.cpp
```

---

**Not:** build/ klasörü .gitignore'da olduğu için push edilmez (doğru!).

Sorun varsa lütfen adımdaki hata mesajını paylaş!

