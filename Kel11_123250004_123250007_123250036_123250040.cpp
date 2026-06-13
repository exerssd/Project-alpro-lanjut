// NIM Kelompok: 123250004 123250007 123250036 123250040
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem> 
#include <iomanip>

using namespace std;
namespace fs = std::filesystem; 

struct Jersey {
    int noPunggung;
    string ukuranBaju;
    string warnaBaju;
};

Jersey arrData[100]; 
int jmlhDataGlobal = 0;

void swapJersey(Jersey& a, Jersey& b) {
    Jersey temp = a;
    a = b;
    b = temp;
}

void bubbleSort(Jersey arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].noPunggung > arr[j+1].noPunggung) {
                swapJersey(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void selectionSort(Jersey arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].noPunggung < arr[minIdx].noPunggung) minIdx = j;
        }
        if (minIdx != i) swapJersey(arr[i], arr[minIdx]);
    }
}

void insertionSort(Jersey arr[], int n) {
    for (int i = 1; i < n; i++) {
        Jersey key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].noPunggung > key.noPunggung) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

void shellSort(Jersey arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Jersey temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap].noPunggung > temp.noPunggung) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

int partition(Jersey arr[], int low, int high) {
    int pivot = arr[high].noPunggung;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].noPunggung <= pivot) {
            i++;
            swapJersey(arr[i], arr[j]);
        }
    }
    swapJersey(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortRec(Jersey arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRec(arr, low, pi - 1);
        quickSortRec(arr, pi + 1, high);
    }
}

void quickSort(Jersey arr[], int n) {
    if (n > 1) quickSortRec(arr, 0, n - 1);
}

void merge(Jersey arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Jersey L[10], R[10];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].noPunggung <= R[j].noPunggung) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortRec(Jersey arr[], int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortRec(arr, l, m);
    mergeSortRec(arr, m + 1, r);
    merge(arr, l, m, r);
}

void mergeSort(Jersey arr[], int n) {
    if (n > 1) mergeSortRec(arr, 0, n - 1);
}

void updateFile(Jersey arr[], int n, string namaFile) {
    fstream file(namaFile, ios::out); 
    if (file.is_open()) {
        for (int i = 0; i < n; i++) {
            file << arr[i].noPunggung << endl;
            file << arr[i].ukuranBaju << endl;
            file << arr[i].warnaBaju << endl;
        }
        file.close();
    } else {
        cout << "Gagal memperbarui database file!" << endl;
    }
}

void loadData(Jersey arr[], int &n, string namaFile) {
    ifstream file(namaFile);
    n = 0;
    if (file.is_open()) {
        while (file >> arr[n].noPunggung) {
            file.ignore(); 
            getline(file, arr[n].ukuranBaju);
            getline(file, arr[n].warnaBaju);
            n++;
        }
        file.close();
    }
}

string pilihFile() {
    string daftarFile[50];
    int jumlahFile = 0;

    cout << "=== DAFTAR FILE YANG TERSEDIA ===" << endl;
    
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            daftarFile[jumlahFile] = entry.path().filename().string();
            jumlahFile++;
            cout << jumlahFile << ". " << entry.path().filename().string() << endl;
        }
    }

    cout << "0. Buat file baru / Masukkan nama file manual" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Pilih nomor file (atau 0): ";
    int pilihan;
    cin >> pilihan;

    if (pilihan > 0 && pilihan <= jumlahFile) {
        return daftarFile[pilihan - 1]; 
    } else {
        string namaManual;
        cout << "Masukkan nama file secara manual (beserta .txt): ";
        cin >> namaManual;
        return namaManual;
    }
}

void InputData() {
    system("cls");
    
    string namaFileTarget = pilihFile();

    loadData(arrData, jmlhDataGlobal, namaFileTarget);

    int banyakDataBaru;
    cout << "\nMasukkan Jumlah Data yang ingin ditambahkan (maks total 100): ";
    cin >> banyakDataBaru;

    if (banyakDataBaru + jmlhDataGlobal > 100 || banyakDataBaru <= 0) {
        cout << "Jumlah data tidak valid atau melebihi kapasitas memori!" << endl;
        system("pause");
        return;
    }

    int batasanBaru = jmlhDataGlobal + banyakDataBaru;
    for (int i = jmlhDataGlobal; i < batasanBaru; i++) {
        cout << "\n--- INPUT DATA KE-" << (i + 1) << " ---" << endl;
        cout << "Nomor Punggung: "; cin >> arrData[i].noPunggung;
        cin.ignore(); 
        cout << "Ukuran Baju   : "; getline(cin, arrData[i].ukuranBaju);
        cout << "Warna Baju    : "; getline(cin, arrData[i].warnaBaju);
    }
    
    jmlhDataGlobal += banyakDataBaru;
    updateFile(arrData, jmlhDataGlobal, namaFileTarget);
    
    cout << "\n Data berhasil diinput dan disimpan ke file \"" << namaFileTarget << "\"!" << endl;
    system("pause");
}

void TampilData() {
	string namaFileTarget = pilihFile();
    loadData(arrData, jmlhDataGlobal, namaFileTarget);
    if (jmlhDataGlobal == 0) {
        cout << "\n Belum ada data yang diinput." << endl;
        return;
    }

    cout << "\n--- DAFTAR DATA  ---" << endl;
    for (int i = 0; i < jmlhDataGlobal; i++) {
        cout << "\nData ke-" << (i + 1) << ":" << endl;
        cout << "  Nomor Punggung: " << arrData[i].noPunggung << endl;
        cout << "  Ukuran Baju   : " << arrData[i].ukuranBaju << endl;
        cout << "  Warna Baju    : " << arrData[i].warnaBaju << endl;
    }
}

void TampilDataSort(){
	 cout<<string(20,'=');
	 cout << "\n--- DAFTAR DATA  ---" << endl;
    for (int i = 0; i < jmlhDataGlobal; i++) {
        cout << "\nData ke-" << (i + 1) << ":" << endl;
        cout << "  Nomor Punggung: " << arrData[i].noPunggung << endl;
        cout << "  Ukuran Baju   : " << arrData[i].ukuranBaju << endl;
        cout << "  Warna Baju    : " << arrData[i].warnaBaju << endl;
		}
	cout<<string(20,'=');
	}

void SeqSearch(Jersey arr[], int n, int nilaiCari) {
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (arr[i].noPunggung == nilaiCari) {
            cout << "Data ditemukan pada indeks ke-" << i << endl;
            cout << "Nomor Punggung : " << arr[i].noPunggung << endl;
            cout << "Ukuran Baju    : " << arr[i].ukuranBaju << endl;
            cout << "Warna Baju     : " << arr[i].warnaBaju << endl;
            cout << "=========================" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Data Tidak Ditemukan" << endl;
    }
}

void BiSearch(Jersey arr[], int n, int nilaiCari) {
    int Idx_kiri, Idx_kanan, mid;
    bool found = false;

    Idx_kiri = 0;
    Idx_kanan = n - 1;

    while (Idx_kiri <= Idx_kanan) {
        mid = Idx_kiri + (Idx_kanan - Idx_kiri) / 2;

        if (arr[mid].noPunggung == nilaiCari) {
            found = true;
            cout << "Data ditemukan pada indeks ke-" << mid << endl; 
            cout << "Nomor Punggung : " << arr[mid].noPunggung << endl;
            cout << "Ukuran Baju    : " << arr[mid].ukuranBaju << endl;
            cout << "Warna Baju     : " << arr[mid].warnaBaju << endl;
            cout << "=========================" << endl;
            break; 
        } 
        else if (nilaiCari < arr[mid].noPunggung) {
            Idx_kanan = mid - 1;
        } 
        else {
            Idx_kiri = mid + 1;
        }
    }

    if (!found) {
        cout << "Data tidak ditemukan" << endl;
    }
}

void MenuSearching() {
    char ulang;
    do {
        system("cls");
        
        string namaFileTarget = pilihFile();

        loadData(arrData, jmlhDataGlobal, namaFileTarget);

        if (jmlhDataGlobal == 0) {
            cout << "\n File \"" << namaFileTarget << "\" kosong atau tidak ditemukan data!\n";
            system("pause");
            return;
        }

        int cariNo, pilihSearch;    
        cout << "\n=== Menu Searching ===" << endl;
        cout << "1. Sequential Search" << endl;
        cout << "2. Binary Search" << endl;
        cout << "Pilih Jenis Search : ";
        cin >> pilihSearch;

        switch(pilihSearch) {
            case 1 :
                cout << "Sequential Search" << endl;
                cout << "NoPunggung Yang Dicari :"; cin >> cariNo;
                SeqSearch(arrData, jmlhDataGlobal, cariNo);
                break;
            case 2 :
                cout << "Binary Search" << endl;
                cout << "NoPunggung Yang Dicari :"; cin >> cariNo;
                BiSearch(arrData, jmlhDataGlobal, cariNo);
                break;
            default: 
                cout << "Menu Tidak Tersedia" << endl;
        }

        cout << "\nUlangi Search? (y/t): ";
        cin >> ulang;

    } while(ulang == 'y' || ulang == 'Y');
}
void SimpanFileSort(Jersey arr[], int &n) {
	char opt;
	char namaFileBaru[50];
	cout<<"Simpan data ini ? (y/n) :";cin>>opt;
	if(opt != 'y' && opt != 'Y'){ 
	return;
	}
	cin.ignore();
	cout<<"Simpan dengan nama file :";cin>>namaFileBaru;
	ofstream filenew(namaFileBaru);
	if(filenew.is_open()){
		for(int i = 0; i<n;i++){
			filenew << arr[i].noPunggung << endl;
			filenew << arr[i].ukuranBaju << endl;
			filenew << arr[i].warnaBaju << endl; 
			}
			filenew.close();
		}
	else{
		cout<<"ERROR MEMBUKA FILE"<<endl;
		}
	cout<<"Data Berhasil di simpan di :"<<namaFileBaru;
	}
	
void MenuSorting() {
    system("cls");
    
    string namaFileTarget = pilihFile();

    loadData(arrData, jmlhDataGlobal, namaFileTarget);

    if (jmlhDataGlobal == 0) {
        cout << "\n File \"" << namaFileTarget << "\" kosong atau tidak ditemukan data!\n";
        system("pause");
        return;
    }

    char ulang;
    do {
        system("cls");
        cout << "=== FILE AKTIF: " << namaFileTarget << " ===" << endl;
        cout << "=== PILIH ALGORITMA SORTING ===" << endl;
        cout << "1. BUBBLE SORT" << endl;
        cout << "2. SELECTION SORT" << endl;
        cout << "3. INSERTION SORT" << endl;
        cout << "4. SHELL SORT" << endl;
        cout << "5. QUICK SORT" << endl;
        cout << "6. MERGE SORT" << endl;
        cout << "0. Kembali ke Menu Utama" << endl;
        cout << "Pilihan: "; 
        int pilihan; 
        cin >> pilihan;
		
        if (pilihan == 0){
         break;
		}
        switch(pilihan) {
            case 1: cout<<string(20,'=')<<endl;;
					cout<<"Data sebelum di sorting : "<<endl;
					TampilDataSort();
					cout<<string(20,'=')<<endl;
					cout<<"Data Setelah di Sorting : "<<endl;
					bubbleSort(arrData, jmlhDataGlobal);
					TampilDataSort();
					break;
            case 2: cout<<string(20,'=')<<endl;;
					cout<<"Data sebelum di sorting : "<<endl;
					TampilDataSort();
					cout<<string(20,'=')<<endl;
					cout<<"Data Setelah di Sorting : "<<endl;
					selectionSort(arrData, jmlhDataGlobal);
					TampilDataSort(); 
					break;
            case 3: cout<<string(20,'=')<<endl;;
					cout<<"Data sebelum di sorting : "<<endl;
					TampilDataSort();
					cout<<string(20,'=')<<endl;
					cout<<"Data Setelah di Sorting : "<<endl;
					insertionSort(arrData, jmlhDataGlobal); 
					TampilDataSort();
					break;
            case 4: cout<<string(20,'=')<<endl;;
					cout<<"Data sebelum di sorting : "<<endl;
					TampilDataSort();
					cout<<string(20,'=')<<endl;
					cout<<"Data Setelah di Sorting : "<<endl;
					shellSort(arrData, jmlhDataGlobal); 
					TampilDataSort();
					break;
            case 5: cout<<string(20,'=')<<endl;;
					cout<<"Data sebelum di sorting : "<<endl;
					TampilDataSort();
					cout<<string(20,'=')<<endl;
					cout<<"Data Setelah di Sorting : "<<endl;
					quickSort(arrData, jmlhDataGlobal); 
					TampilDataSort();
					break;
            case 6: cout<<string(20,'=')<<endl;;
					cout<<"Data sebelum di sorting : "<<endl;
					TampilDataSort();
					cout<<string(20,'=')<<endl;
					cout<<"Data Setelah di Sorting : "<<endl;
					mergeSort(arrData, jmlhDataGlobal); 
					TampilDataSort();
					break;
            default: 
                cout << "\n Pilihan tidak valid!\n";
                system("pause");
                continue;
        }
        SimpanFileSort(arrData, jmlhDataGlobal);
  
        cout << "\nUlangi pemilihan sorting? (y/t): ";
        cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y'); 

    system("cls");
}

void HapusData(Jersey arr[], int &n, string namaFile) {
    int cari;
    cout << "\n=== HAPUS ===" << endl;
    cout << "Masukkan No Jersey yg ingin dihapus : "; cin >> cari;
    bool ketemu = false;
    
    for (int i = 0; i < n; i++) {
        if (arr[i].noPunggung == cari) {
            for (int j = i; j < n - 1; j++) {
                arr[j] = arr[j + 1];
            }
            n--; 
            cout << "Jersey dengan no " << cari << " berhasil dihapus dari sistem!\n";
            ketemu = true;
            
            updateFile(arr, n, namaFile);
            cout << "Database file \"" << namaFile << "\" berhasil diperbarui!\n";
			break;
        }
    }
    if (!ketemu) {
        cout << "Jersey dengan no " << cari << " tidak ditemukan!\n";
    }
    system("pause"); 
}

void MenuOperasi() {
    int pilih;
    system("cls");
    
    string namaFileTarget = pilihFile();

    loadData(arrData, jmlhDataGlobal, namaFileTarget);

    cout << "\n=== OPERASI FILE VIA: " << namaFileTarget << " ===" << endl;
    cout << "1. Hapus Data " << endl;
    cout << "2. Kembali ke Menu Utama" << endl;
    cout << "Pilih : "; cin >> pilih;
    switch(pilih) {
        case 1 : 
            if(jmlhDataGlobal == 0) {
                cout << "\nBelum ada data untuk dihapus!\n";
                system("pause");
            } else {
                HapusData(arrData, jmlhDataGlobal, namaFileTarget);
            }
            break;
        case 2 : 
            break;
        default : 
            cout << "Menu Tidak Tersedia" << endl;
            system("pause");
    }
}


int main() {
    int menu;
    do {
        system("cls");
        cout << "==== MENU UTAMA ====" << endl;
        cout << "1. INPUT DATA" << endl;
        cout << "2. TAMPIL DATA" << endl;
        cout << "3. SEARCHING" << endl;
        cout << "4. SORTING" << endl;
        cout << "5. OPERASI FILE" << endl;
        cout << "6. EXIT" << endl; 
        cout << "=====================" << endl;
        cout << "PILIH MENU: "; cin >> menu;
        switch (menu) {
            case 1: InputData(); break;
            case 2: 
                system("cls"); 
                TampilData(); 
                system("pause"); 
                break;
            case 3: MenuSearching(); break;
            case 4: MenuSorting(); break;
            case 5: MenuOperasi(); break; 
            case 6: cout << "\nKeluar dari program. Terima kasih!\n"; break;
            default: cout << "\n Menu tidak valid\n"; system("pause");
        }
    } while (menu != 6); 

    return 0;
}
