# Stack Overflow

## Penjelasan Logika Fitur:

1.  **Guest vs Member :**

- User yang belum login dianggap sebagai **Guest** dan yang sudah login dianggap sebagai **Member**.
- **Guest:** Bisa mengakses menu `Show Newest Questions` dan `Search Questions` untuk membaca konten.
- **Member:** Saat memilih `Show Profile` atau `Ask Question`, sistem mengecek status login. Menu interaksi seperti _Vote_, _Answer_, dan _Comment_ hanya muncul atau bisa dieksekusi jika user sudah jadi Member.

2.  **Detail Pertanyaan & Jawaban:**

- Di dalam **[Show Question]**, aplikasi akan menampilkan isi pertanyaan dan klik nomer pertanyaan untuk melihat jawaban.
- Tiap jawaban memiliki indikator apakah sudah **Verified** (diterima oleh penanya) atau belum.
- Member dapat melakukan aksi _Vote Up_ atau _Vote Down_ baik pada pertanyaan utama maupun pada setiap jawaban yang ada.

3.  **Search & Filtering:**

- Menu search menyediakan sub-menu untuk mengatur "Parameter" sebelum pencarian dilakukan.
- Filter **Verified** membantu pengguna menemukan solusi yang sudah pasti benar (diterima oleh penanya).
- Sorting **Vote (ASC/DESC)** memungkinkan pengguna melihat solusi yang paling populer (banyak upvote) di posisi teratas.

## Flow of Activity

### Post a question

1. Masukkan title (judul pertanyaan) dan body (deskripsi masalahnya)
2. Tambah tags (wajib diisi, minimal 1 maks 5)

### Post an answer

1. Pilih question
2. Tulis answer

## Menu

1. Show profile
2. Show newest questions
3. Search questions
4. Ask question
5. Exit

## Penjelasan Tiap Menu

```text
MENU/
├── 👤 1. Show Profile
│ ├── 🔒 If Guest (Not Logged In)
│ │ ├── 📝 Register (Daftar menjadi Member)
│ │ ├── 🔑 Login
│ │ └── 🔙 Back to Main Menu
│ └── 🔓 If Member (Logged In)
│ ├── 📊 View Stats (Username, Reputasi, Badge)
│ ├── 📚 My Questions (List pertanyaan sendiri)
│ ├── 📜 My Answers (List jawaban sendiri)
│ └── 🔙 Back to Main Menu
│
├── 🆕 2. Show Newest Questions
│ ├── 📄 List: [Title] | [Votes] | [Answers] | [Is Verified?]
│ └── 🔍 Select Question (ID)
│ └── 📖 [Question Detail View]
│ ├── 💬 View Comments
│ ├── 💡 View Answers (List jawaban + Status Verifikasi)
│ └── 🛠️ Actions (Member Only)
│ ├── 👍 Vote Up / 👎 Vote Down (Question)
│ ├── 💬 Add Comment
│ ├── 📝 Post Answer
│ └── 🗳️ Vote Answer (Up/Down)
│
├── 🔍 3. Search Questions
│ ├── ⌨️ Enter Keyword
│ ├── ⚙️ Set Filters (Optional)
│ │ ├── 🔢 Min Votes Count (e.g., > 10)
│ │ ├── ❓ Answer Status (All / Answered / Unanswered)
│ │ └── ✅ Verification Status (Verified / Not Verified)
│ ├── 📑 Set Sorting
│ │ ├── ⬆️ Vote (ASC)
│ │ └── ⬇️ Vote (DESC)
│ ├── 🚀 Execute Search
│ └── 📄 Results List (Sama seperti menu Newest Questions)
│ └── 🔍 Select Question -> [Question Detail View]
│
├── ❓ 4. Ask Question
│ ├── 🔒 If Guest
│ │ └── ⚠️ "Please Login/Register first to ask a question"
│ │ ├── 📝 Register
│ │ └── 🔙 Back
│ └── 🔓 If Member
│ ├── ⌨️ Input Title
│ ├── ⌨️ Input Content/Body
│ ├── ⌨️ Input Tags
│ └── ✅ Submit Question
│
└── 🚪 5. Exit (Keluar Aplikasi)
```

## Cara Pake Function

1. Menampilkan profile user berdasarkan ID.
   ```c
   app.showProfile(<userId>) // Menampilkan user berdasarkan user ID
   
   app.createUser(<nama>, <password>, <email>) // Membuat user baru
   
   app.createQuestion(<userId>) // Membuat pertanyaan berdasarkan user ID.
   
   app.voteUp(<struct Question/Answer>, <userId>) // Vote pertnyaan atau jawaban berdasarkan user ID.
   
   app.voteDown(<struct Question/Answer>, <userId>) // vote down pertanyaan atau jawaban berdasarkan user ID.
   
   app.assignBadges(<userId>) // Memberikan badges berdasarkan user ID (jenis badges ditentukan otomatis oleh function).
   
   app.assignReputation(<userId>) // Memberikan reputasi berdasarkan user ID.
   ```
