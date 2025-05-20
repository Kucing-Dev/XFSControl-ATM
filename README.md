# C++ untuk Mengontrol Cash Dispenser dan Logging (XFS API) 

## Apa itu Cash Dispenser?
Bayangin lo lagi narik duit di ATM, terus tiba-tiba "jreng!" — duit keluar dari mesin.
Nah, yang ngeluarin duit itu namanya Cash Dispenser. Itu kayak tangan robot si ATM yang ambil uang dari kaset (kotak uang di dalam) dan nyodorin ke kita.

Intinya:
> Cash Dispenser = Bagian mesin ATM yang tugasnya ngeluarin duit

## Lalu, XFS API itu apa sih?
XFS itu singkatan dari eXtensions for Financial Services.

Gampangnya:
> XFS API itu alat bantu buat developer kayak lo biar bisa ngontrol semua perangkat keras di mesin ATM pake cara yang standar & konsisten, gak peduli mereknya beda-beda.

 ## Apa aja yang bisa XFS kontrol?
 
| Hardware   | Fungsi                           
|------------|----------------------------------|
| CDM        | Cash Dispenser (ngeluarin duit)  | 
| PTR        | Receipt Printer (Ngeprint struk) |
| IDC        | Card Reader (Baca/tarik kartu)   | 
| PIN (PIN Pad)| Input PIN                      |          
|CRM (Cash Recycler) | Masukin & ngeluarin duit |

 ### Cara kerja XFS kira-kira gini:

| Fungsi XFS API         | Keterangan                                                                  |
|------------------------|----------------------------------------------------------------------------------|
| `WFSStartUp()`         | Nyalain koneksi ke XFS Manager (kayak pusat kontrol ATM)                         |
| `WFSOpen("ATMCDM")`    | Buka koneksi ke hardware spesifik, misalnya ke cash dispenser                    |
| `WFSGetInfo()` / `WFSGetStatus()` | Cek status alatnya: siap, error, rusak, atau offline?                            |
| `WFSExecute()`         | Kirim perintah buat ngeluarin duit, baca kartu, ngeprint struk, dll              |
| `WFSFreeResult()`      | Beresin memori setelah dapet hasil dari hardware (biar ga bocor memori)         |
| `WFSClose()` + `WFSCleanUp()` | Tutup koneksi dengan baik              |


 ## Logging itu buat apa?
 Bayangin ATM ngeluarin duit, terus lo ga catet. Bisa-bisa ada transaksi misterius dan ga bisa dicek siapa yang salah.

Makanya:

> Logging itu kayak buku harian si ATM, biar semua kejadian dicatet.
> Contoh: "User tarik Rp100.000, berhasil." atau "Cash Dispenser error E302."


### *Penjelasan Kode:*

1. *Inisialisasi XFS*:

   * Menggunakan WFSStartUp() untuk memulai library XFS.
   * WFSOpen() digunakan untuk membuka koneksi ke cash dispenser (biasanya dengan logical name seperti "ATMCDM").

2. *Mengecek Status Kaset Uang*:

   * WFSGetStatus() digunakan untuk memeriksa status perangkat cash dispenser.
   * Kode ini mengecek apakah perangkat ATM *online* dan siap untuk melakukan operasi.

3. *Proses Dispense Uang*:

   * Menyiapkan struktur WFSCDMDISPENSE untuk melakukan operasi dispense uang dari ATM.
   * fwPosition mengontrol apakah uang dikeluarkan dari posisi depan atau belakang.
   * usMixNumber mengontrol *pecahan uang* yang dikeluarkan, misalnya: 1 untuk pecahan tertentu (konfigurasi di ATM).

4. *Logging*:

   * Fungsi logMessage() digunakan untuk mencatat semua kegiatan dan hasil analisis di file log (atm_malware_log.txt).
   * Ini akan mempermudah pelacakan dan audit dari apa yang terjadi selama malware beroperasi.

5. *Menutup dan Membersihkan*:

   * Setelah selesai, koneksi ke service XFS ditutup dengan WFSClose().
   * WFSCleanUp() digunakan untuk membersihkan library XFS setelah selesai.

### Segala bentuk tindak kriminal saya tidak bertanggung jawab
> ini cuma edukasi soalnya :v
