#include <sstream>
#include <vector>
#include <cstdlib>

#include "danh_sach_lien_ket_don.h"
#include "danh_sach_lien_ket_kep.h"
#include "danh_sach_lien_ket_vong.h"
#include "mang.h"
#include "nhapXuatSinhVien.h"
#include "xoaSinhVien.h"
#include "lietKe_TinhDiem.h"

#define BLUE 9
#define CYAN 11
#define WHITE 15
#define RED 4

// ------------------------------------

// Hàm đổi màu chữ
void SET_COLOR(int color)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

enum class Field
{
    MaSV,
    Ho,
    Ten,
    Lop,
    Diem
};
template <typename T>
T getValue(const SV& sv, const string& field)
{
    if (field == "maSV")
        return sv.maSV;
    if (field == "ho")
        return sv.ho;
    if (field == "ten")
        return sv.ten;
    if (field == "lop")
        return sv.lop;
    if (field == "diem") // Thêm trường hợp cho trường "diem"
        return to_string(sv.diem);
    throw invalid_argument("Field name is invalid");
}

string getValue(const SV& sv, const string& field)
{
    std::ostringstream stream;
    if (field == "maSV")
        return sv.maSV;
    if (field == "ho")
        return sv.ho;
    if (field == "ten")
        return sv.ten;
    if (field == "lop")
        return sv.lop;
    if (field == "diem")
    {
        stream << std::fixed << std::setprecision(2) << sv.diem;
        return stream.str();
    }
    throw invalid_argument("Field name is invalid");
}

template <typename T>
int Binary_Search(SV listMang[], int left, int right, const T& x, const string& input)
{
    if (left > right)
    {
        return -1;
    }
    int mid = (left + right) / 2;
    string midValue = getValue(listMang[mid], input);

    stringstream ss;
    ss << x;
    string xAsString = ss.str();

    if (midValue == xAsString)
    {
        return mid;
    }
    else if (midValue < xAsString)
    {
        return Binary_Search(listMang, mid + 1, right, xAsString, input);
    }
    else
    {
        return Binary_Search(listMang, left, mid - 1, xAsString, input);
    }
}

template <typename T2>
vector<int> Linear_Search_Substring(SV listMang[], int size, const T2& substring, const string& field)
{
    vector<int> indices;
    for (int i = 0; i < size; ++i)
    {
        string fieldValue = getValue(listMang[i], field);
        // Check if the substring is part of the fieldValue
        if (fieldValue.find(substring) != string::npos)
        {
            indices.push_back(i);
        }
    }
    return indices;
}

// quick sort
void quickSort(SV a[], int l, int r, const string& input)
{

    string midValue = getValue(a[(l + r) / 2], input);
    // int p = a[(l+r)/2].m;
    int i = l, j = r;
    while (i < j)
    {
        while (getValue(a[i], input) < midValue)
        {
            i++;
        }
        while (getValue(a[j], input) > midValue)
        {
            j--;
        }
        if (i <= j)
        {
            SV temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            i++;
            j--;
        }
    }
    if (i < r)
    {
        quickSort(a, i, r, input);
    }
    if (l < j)
    {
        quickSort(a, l, j, input);
    }
}

// ----------------------------------------Phần dành cho DSLK Đơn

//----------------
string TenDaoNguoc(string str)
{
    string temp = str;
    reverse(temp.begin(), temp.end());
    return temp;
}

// ------------- mảng
void luaChonXuatTenDaoNguocMang(SV LIST_MANG[], int& soLuongSinhVien, const vector<int>& foundIndices, int& index, int thoiGianTimKiem, const string& field, const string& value)
{
    int lc;
    bool backToSearchMenu = false; // Biến để kiểm tra liệu người dùng muốn quay lại menu tìm kiếm ban đầu hay không
    while (!backToSearchMenu)      // Thực hiện trong khi người dùng không muốn quay lại menu tìm kiếm
    {
        system("cls");
        cout << "\tDa tim thay thong tin sinh vien. Ban co muon xuat ten dao nguoc khong?";
        cout << "\n\t  1. YES";
        cout << "\n\t  2. NO";
        cout << "\n\t  3. Quay lai menu tim kiem";
        cout << "\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            inTieuDe();
            for (int i : foundIndices)
            {
                string reversedName = TenDaoNguoc(LIST_MANG[i].ten);
                string reversedHo = TenDaoNguoc(LIST_MANG[i].ho);

                // Kiểm tra nếu "ho" bắt đầu bằng giá trị tìm kiếm
                if (field == "ho" && LIST_MANG[i].ho.find(value) == 0)
                {
                    SET_COLOR(RED);
                    xuat(LIST_MANG[i]);
                    SET_COLOR(WHITE); // Thay WHITE bằng mã màu mặc định của bạn
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                }
                else if (field != "ho" && getValue(LIST_MANG[i], field).find(value) != std::string::npos)
                {
                    SET_COLOR(RED);
                    xuat(LIST_MANG[i]);
                    SET_COLOR(WHITE); // Thay WHITE bằng mã màu mặc định của bạn
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                }
            }

            cout << "\n\t--------Thoi gian tim kiem-------: " << thoiGianTimKiem << endl;
            system("pause");
            break;
        }

        case 2:
        {
            inTieuDe();
            for (int i : foundIndices)
            {
                // Kiểm tra nếu "ho" bắt đầu bằng giá trị tìm kiếm
                if (field == "ho" && LIST_MANG[i].ho.find(value) == 0)
                {
                    SET_COLOR(RED);
                    xuat(LIST_MANG[i]);
                    SET_COLOR(WHITE); // Thay WHITE bằng mã màu mặc định của bạn
                }
                else if (field != "ho" && getValue(LIST_MANG[i], field).find(value) != std::string::npos)
                {
                    SET_COLOR(RED);
                    xuat(LIST_MANG[i]);
                    SET_COLOR(WHITE); // Thay WHITE bằng mã màu mặc định của bạn
                }
            }
            cout << "\n\t--------Thoi gian tim kiem-------: " << thoiGianTimKiem << endl;

            system("pause");
            break;
        }

        case 3:
        {
            backToSearchMenu = true; // Đặt biến backToSearchMenu về true để thoát khỏi vòng lặp và quay lại menu tìm kiếm
            break;
        }

        default:
            break;
        }
    }
}

//----------------
void timKiemSinhVienMang(SV LIST_MANG[], int& soLuongSinhVien) //  ---------- MẢNG
{
    int lc;
    // int tgianTimKiem;
    bool backToMenu = false;
    while (!backToMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH TIM KIEM SINH VIEN ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t  1. Tim theo ma sinh vien.";
        cout << "\n\t  2. Tim theo ho.";
        cout << "\n\t  3. Tim theo ten.";
        cout << "\n\t  4. Tim theo lop.";
        cout << "\n\t  5. Tim theo diem.";
        cout << "\n\t  10. Quay lai.";
        cout << "\n\t  0. Thoat.";
        cout << "\n\t======================= END =======================";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            cin.ignore();
            int index = 0;
            cout << "\n\tNhap ma sinh vien can tim : ";
            string mssvCanTim = "";
            getline(cin, mssvCanTim);
            formMssv(mssvCanTim);
            // --- sap xep
            quickSort(LIST_MANG, 0, soLuongSinhVien - 1, "maSV");

            //-------------------
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            int result_str = Binary_Search<string>(LIST_MANG, 0, soLuongSinhVien - 1, mssvCanTim, "maSV");

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            if (result_str != -1)
            {
                vector<int> foundIndices;
                foundIndices.push_back(result_str);
                luaChonXuatTenDaoNguocMang(LIST_MANG, soLuongSinhVien, foundIndices, index, tgianTimKiem, "maSV", mssvCanTim);

                system("pause");
            }
            else
            {
                cout << "Not found" << endl;
            }

            system("pause");
            break;
        }

        case 2:
        {
            cin.ignore();
            int index = 0;
            cout << "\n\tNhap ho sinh vien can tim : ";
            string hoCanTim = "";
            getline(cin, hoCanTim);
            formChu(hoCanTim);

            // --- sap xep thử
            quickSort(LIST_MANG, 0, soLuongSinhVien - 1, "ho");

            //-------------------

            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            vector<int> foundIndices = Linear_Search_Substring(LIST_MANG, soLuongSinhVien, hoCanTim, "ho");

            auto end = chrono::high_resolution_clock::now();                         // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            if (!foundIndices.empty())
            {

                luaChonXuatTenDaoNguocMang(LIST_MANG, soLuongSinhVien, foundIndices, index, tgianTimKiem, "ho", hoCanTim);
            }
            else
            {
                cout << "No matches found." << endl;
            }

            system("pause");
            break;
        }

        case 3:
        {
            cin.ignore();
            int index = 0;
            cout << "\n\tNhap ten sinh vien can tim : ";
            string tenCanTim = "";
            getline(cin, tenCanTim);
            formChu(tenCanTim);

            // --- sap xep thử
            quickSort(LIST_MANG, 0, soLuongSinhVien - 1, "ten");

            //-------------------

            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            int result_str = Binary_Search<string>(LIST_MANG, 0, soLuongSinhVien - 1, tenCanTim, "ten");

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            if (result_str != -1)
            {
                vector<int> foundIndices;
                for (int i = 0; i < soLuongSinhVien; ++i)
                {
                    if (LIST_MANG[i].ten == tenCanTim)
                    {
                        foundIndices.push_back(i);
                    }
                }
                luaChonXuatTenDaoNguocMang(LIST_MANG, soLuongSinhVien, foundIndices, index, tgianTimKiem, "ten", tenCanTim);

                system("pause");
            }
            else
            {
                cout << "Not found" << endl;
            }

            system("pause");
            break;
        }

        case 4:
        {
            cin.ignore();
            int index = 0;
            cout << "\n\tNhap lop sinh vien can tim : ";
            string lopCanTim = "";
            getline(cin, lopCanTim);
            formMssv(lopCanTim);
            // --- sap xep thử
            quickSort(LIST_MANG, 0, soLuongSinhVien - 1, "lop");

            //-------------------

            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            int result_str = Binary_Search<string>(LIST_MANG, 0, soLuongSinhVien - 1, lopCanTim, "lop");

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            if (result_str != -1)
            {
                vector<int> foundIndices;
                for (int i = 0; i < soLuongSinhVien; ++i)
                {
                    if (LIST_MANG[i].lop == lopCanTim)
                    {
                        foundIndices.push_back(i);
                    }
                }
                luaChonXuatTenDaoNguocMang(LIST_MANG, soLuongSinhVien, foundIndices, index, tgianTimKiem, "lop", lopCanTim);

                system("pause");
            }
            else
            {
                cout << "Not found" << endl;
            }

            system("pause");
            break;
        }

        case 5:
        {
            int index = 0;
            cout << "\n\tNhap diem sinh vien can tim : ";
            float diemCanTim;
            cin >> diemCanTim;

            // --- sap xep thử
            quickSort(LIST_MANG, 0, soLuongSinhVien - 1, "diem");

            //-------------------

            // tìm
            int result_str = Binary_Search<float>(LIST_MANG, 0, soLuongSinhVien - 1, diemCanTim, "diem");
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            vector<int> foundIndices;
            for (int i = 0; i < soLuongSinhVien; ++i)
            {
                if (LIST_MANG[i].diem == diemCanTim)
                {
                    foundIndices.push_back(i);
                }
            }
            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            // Chuyển đổi float thành string
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << diemCanTim;
            std::string diemCanTimStr = ss.str();
            luaChonXuatTenDaoNguocMang(LIST_MANG, soLuongSinhVien, foundIndices, index, tgianTimKiem, "diem", diemCanTimStr);

            system("pause");
            break;
        }

        case 10:
        {
            backToMenu = true;
            break;
        }

        case 0:
        {
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        }
        }
    }
}

//--------- dslk đơn
void luaChonXuatTenDaoNguocDSLKDon(LIST_DON listDon, const string& field, const string& value)
{
    int lc;
    bool backToSearchMenu = false;

    while (!backToSearchMenu)
    {
        system("cls"); // Xóa màn hình
        cout << "\tDa tim thay thong tin sinh vien. Ban co muon xuat ten dao nguoc khong?";
        cout << "\n\t  1. YES";
        cout << "\n\t  2. NO";
        cout << "\n\t  3. Quay lai menu tim kiem";
        cout << "\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            inTieuDe();
            node_Don* p = NULL;

            p = listDon.pHead_Don;
            while (p != NULL)
            {

                // Kiểm tra nếu trường đang tìm kiếm là "ho" và giá trị của "ho" bắt đầu bằng giá trị tìm kiếm
                if (field == "ho" && getValue(p->data, field).find(value) == 0)
                {
                    string reversedName = TenDaoNguoc(p->data.ten);
                    string reversedHo = TenDaoNguoc(p->data.ho);
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }
                // Kiểm tra nếu trường đang tìm kiếm không phải là "ho" và giá trị khớp với giá trị tìm kiếm
                else if (field != "ho" && getValue(p->data, field).find(value) != std::string::npos)
                {
                    string reversedName = TenDaoNguoc(p->data.ten);
                    string reversedHo = TenDaoNguoc(p->data.ho);
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }

                p = p->pNext_Don;
            }

            system("pause");
            break;
        }

        case 2:
        {
            inTieuDe();
            node_Don* p = NULL;

            p = listDon.pHead_Don;
            while (p != NULL)
            {

                if (field == "ho" && getValue(p->data, field).find(value) == 0)
                {

                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }
                // Kiểm tra nếu trường đang tìm kiếm không phải là "ho" và giá trị khớp với giá trị tìm kiếm
                else if (field != "ho" && getValue(p->data, field).find(value) != std::string::npos)
                {
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }

                p = p->pNext_Don;
            }

            system("pause");
            break;
        }

        case 3:
        {
            backToSearchMenu = true;
            break;
        }

        default:
            break;
        }
    }
}

void giaiPhongBoNhoDSLKDon(LIST_DON& listDon)
{
    pNODE_DON p = listDon.pHead_Don;
    while (p != NULL)
    {
        pNODE_DON temp = p;
        p = p->pNext_Don;
        delete temp;
    }
    listDon.pHead_Don = NULL; // Đảm bảo danh sách rỗng
}

void timKiemSinhVienDanhSachLkDon(LIST_DON& listDon)
{
    int lc;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH TIM KIEM SINH VIEN ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t  1. Tim theo ma sinh vien.";
        cout << "\n\t  2. Tim theo ho.";
        cout << "\n\t  3. Tim theo ten.";
        cout << "\n\t  4. Tim theo lop.";
        cout << "\n\t  5. Tim theo diem.";
        cout << "\n\t  10. Quay lai.";
        cout << "\n\t  0. Thoat.";
        cout << "\n\t======================= END =======================";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            cin.ignore();
            cout << "\n\tNhap ma sinh vien can tim : ";
            string mssvCanTim = "";
            getline(cin, mssvCanTim);
            formMssv(mssvCanTim);

            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKDon(listDon, "maSV", mssvCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 2:
        {
            cin.ignore();
            cout << "\n\tNhap ho sinh vien can tim : ";
            string hoCanTim = "";
            getline(cin, hoCanTim);
            formChu(hoCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKDon(listDon, "ho", hoCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 3:
        {
            cin.ignore();
            cout << "\n\tNhap ten sinh vien can tim : ";
            string tenCanTim = "";
            getline(cin, tenCanTim);
            formChu(tenCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            luaChonXuatTenDaoNguocDSLKDon(listDon, "ten", tenCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 4:
        {
            cin.ignore();
            cout << "\n\tNhap lop sinh vien can tim : ";
            string lopCanTim = "";
            getline(cin, lopCanTim);
            formMssv(lopCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKDon(listDon, "lop", lopCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 5:
        {
            cout << "\n\tNhap diem sinh vien can tim : ";
            float diemCanTim;
            cin >> diemCanTim;
            ostringstream convert;
            convert << fixed << setprecision(2) << diemCanTim; // Make sure this precision matches what getValue uses
            string tam = convert.str();
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            luaChonXuatTenDaoNguocDSLKDon(listDon, "diem", tam);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 10:
        {
            backToMainMenu = true;
            break;
        }

        case 0:
        {
            giaiPhongBoNhoDSLKDon(listDon);
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        }
        }
    }
}

//--------- dslk vòng
void luaChonXuatTenDaoNguocDSLKVong(LIST_VONG& listVong, const string& field, const string& value)
{
    int lc;
    bool backToSearchMenu = false;

    while (!backToSearchMenu)
    {
        system("cls"); // Xóa màn hình
        cout << "\tDa tim thay thong tin sinh vien. Ban co muon xuat ten dao nguoc khong?";
        cout << "\n\t  1. YES";
        cout << "\n\t  2. NO";
        cout << "\n\t  3. Quay lai menu tim kiem";
        cout << "\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            inTieuDe();
            node_Vong* p = NULL;
            p = listVong.pTail_Vong;
            do
            {

                if (field == "ho" && getValue(p->data, field).find(value) == 0)
                {
                    string reversedName = TenDaoNguoc(p->data.ten);
                    string reversedHo = TenDaoNguoc(p->data.ho);
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }
                // Kiểm tra nếu trường đang tìm kiếm không phải là "ho" và giá trị khớp với giá trị tìm kiếm
                else if (field != "ho" && getValue(p->data, field).find(value) != std::string::npos)
                {
                    string reversedName = TenDaoNguoc(p->data.ten);
                    string reversedHo = TenDaoNguoc(p->data.ho);
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }

                p = p->pNext_Vong;
            } while (p != listVong.pTail_Vong);
            system("pause");
            break;
        }

        case 2:
        {
            inTieuDe();
            node_Vong* p = NULL;
            p = listVong.pTail_Vong;
            do
            {
                // if (getValue(p->data, field) == value)
                //{
                //     SET_COLOR(RED);
                //     xuat(p->data);
                //     SET_COLOR(WHITE); // Thay WHITE bằng mã màu mặc định của bạn
                // }

                if (field == "ho" && getValue(p->data, field).find(value) == 0)
                {

                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }
                // Kiểm tra nếu trường đang tìm kiếm không phải là "ho" và giá trị khớp với giá trị tìm kiếm
                else if (field != "ho" && getValue(p->data, field).find(value) != std::string::npos)
                {

                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }

                p = p->pNext_Vong;
            } while (p != listVong.pTail_Vong);
            system("pause");
            break;
        }

        case 3:
        {
            backToSearchMenu = true;
            break;
        }

        default:
            break;
        }
    }
}

void giaiPhongBoNhoDSLKVong(LIST_VONG& listVong)
{
    if (listVong.pTail_Vong != NULL)
    {
        pNODE_VONG p = listVong.pTail_Vong->pNext_Vong; // Bắt đầu từ phần tử đầu tiên
        while (p != NULL)
        {
            pNODE_VONG temp = p;
            p = p->pNext_Vong;
            delete temp; // Giải phóng bộ nhớ cho từng phần tử
        }
        listVong.pTail_Vong = NULL; // Đảm bảo rằng pTail trỏ đến NULL sau khi giải phóng
    }
}

void timKiemSinhVienDanhSachLkVong(LIST_VONG& listVong)
{
    int lc;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH TIM KIEM SINH VIEN ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t  1. Tim theo ma sinh vien.";
        cout << "\n\t  2. Tim theo ho.";
        cout << "\n\t  3. Tim theo ten.";
        cout << "\n\t  4. Tim theo lop.";
        cout << "\n\t  5. Tim theo diem.";
        cout << "\n\t  10. Quay lai.";
        cout << "\n\t  0. Thoat.";
        cout << "\n\t======================= END =======================";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            cin.ignore();
            cout << "\n\tNhap ma sinh vien can tim : ";
            string mssvCanTim = "";
            getline(cin, mssvCanTim);
            formMssv(mssvCanTim);

            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKVong(listVong, "maSV", mssvCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 2:
        {
            cin.ignore();
            cout << "\n\tNhap ho sinh vien can tim : ";
            string hoCanTim = "";
            getline(cin, hoCanTim);
            formChu(hoCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKVong(listVong, "ho", hoCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 3:
        {
            cin.ignore();
            cout << "\n\tNhap ten sinh vien can tim : ";
            string tenCanTim = "";
            getline(cin, tenCanTim);
            formChu(tenCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            luaChonXuatTenDaoNguocDSLKVong(listVong, "ten", tenCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 4:
        {
            cin.ignore();
            cout << "\n\tNhap lop sinh vien can tim : ";
            string lopCanTim = "";
            getline(cin, lopCanTim);
            formMssv(lopCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKVong(listVong, "lop", lopCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 5:
        {
            cout << "\n\tNhap diem sinh vien can tim : ";
            float diemCanTim;
            cin >> diemCanTim;
            ostringstream convert;
            convert << fixed << setprecision(2) << diemCanTim; // Make sure this precision matches what getValue uses
            string tam = convert.str();
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            luaChonXuatTenDaoNguocDSLKVong(listVong, "diem", tam);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 10:
        {
            backToMainMenu = true;
            break;
        }

        case 0:
        {
            giaiPhongBoNhoDSLKVong(listVong);
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        }
        }
    }
}

//--------- dslk kép
void luaChonXuatTenDaoNguocDSLKKep(LIST_KEP& listKep, const string& field, const string& value)
{
    int lc;
    bool backToSearchMenu = false;

    while (!backToSearchMenu)
    {
        system("cls"); // Xóa màn hình
        cout << "\tDa tim thay thong tin sinh vien. Ban co muon xuat ten dao nguoc khong?";
        cout << "\n\t  1. YES";
        cout << "\n\t  2. NO";
        cout << "\n\t  3. Quay lai menu tim kiem";
        cout << "\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            inTieuDe();
            node_Kep* p = listKep.pHead_Kep; // Truy cập vào phần đầu danh sách liên kết kép
            while (p != NULL)
            {
                if (field == "ho" && getValue(p->data, field).find(value) == 0)
                {
                    string reversedName = TenDaoNguoc(p->data.ten);
                    string reversedHo = TenDaoNguoc(p->data.ho);
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }
                // Kiểm tra nếu trường đang tìm kiếm không phải là "ho" và giá trị khớp với giá trị tìm kiếm
                else if (field != "ho" && getValue(p->data, field).find(value) != std::string::npos)
                {
                    string reversedName = TenDaoNguoc(p->data.ten);
                    string reversedHo = TenDaoNguoc(p->data.ho);
                    cout << "Ten dao nguoc: " << reversedName << " " << reversedHo << endl;
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }

                p = p->pNext_Kep;
            }
            system("pause");
            break;
        }

        case 2:
        {
            inTieuDe();
            node_Kep* p = listKep.pHead_Kep; // Truy cập vào phần đầu danh sách liên kết kép
            while (p != NULL)
            {
                if (field == "ho" && getValue(p->data, field).find(value) == 0)
                {
                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }
                // Kiểm tra nếu trường đang tìm kiếm không phải là "ho" và giá trị khớp với giá trị tìm kiếm
                else if (field != "ho" && getValue(p->data, field).find(value) != std::string::npos)
                {

                    SET_COLOR(RED);
                    xuat(p->data);
                    SET_COLOR(WHITE);
                }

                p = p->pNext_Kep;
            }
            system("pause");
            break;
        }

        case 3:
        {
            backToSearchMenu = true;
            break;
        }

        default:
            cout << "\nLua chon khong hop le. Vui long nhap lai.";
            system("pause");
            break;
        }
    }
}

void giaiPhongBoNhoDSLKKep(LIST_KEP& listKep)
{
    pNODE_KEP p = listKep.pHead_Kep;
    while (p != NULL)
    {
        pNODE_KEP temp = p;
        p = p->pNext_Kep;
        delete temp;
    }
    listKep.pHead_Kep = listKep.pTail_Kep = NULL; // Đảm bảo danh sách đã được giải phóng
}

void timKiemSinhVienDanhSachLKKep(LIST_KEP& listKep)
{
    int lc;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH TIM KIEM SINH VIEN ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t  1. Tim theo ma sinh vien.";
        cout << "\n\t  2. Tim theo ho.";
        cout << "\n\t  3. Tim theo ten.";
        cout << "\n\t  4. Tim theo lop.";
        cout << "\n\t  5. Tim theo diem.";
        cout << "\n\t  10. Quay lai.";
        cout << "\n\t  0. Thoat.";
        cout << "\n\t======================= END =======================";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> lc;

        switch (lc)
        {
        case 1:
        {
            cin.ignore();
            cout << "\n\tNhap ma sinh vien can tim : ";
            string mssvCanTim = "";
            getline(cin, mssvCanTim);
            formMssv(mssvCanTim);

            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKKep(listKep, "maSV", mssvCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 2:
        {
            cin.ignore();
            cout << "\n\tNhap ho sinh vien can tim : ";
            string hoCanTim = "";
            getline(cin, hoCanTim);
            formChu(hoCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKKep(listKep, "ho", hoCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 3:
        {
            cin.ignore();
            cout << "\n\tNhap ten sinh vien can tim : ";
            string tenCanTim = "";
            getline(cin, tenCanTim);
            formChu(tenCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            luaChonXuatTenDaoNguocDSLKKep(listKep, "ten", tenCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 4:
        {
            cin.ignore();
            cout << "\n\tNhap lop sinh vien can tim : ";
            string lopCanTim = "";
            getline(cin, lopCanTim);
            formMssv(lopCanTim);
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian

            luaChonXuatTenDaoNguocDSLKKep(listKep, "lop", lopCanTim);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 5:
        {
            cout << "\n\tNhap diem sinh vien can tim : ";
            float diemCanTim;
            cin >> diemCanTim;
            ostringstream convert;
            convert << fixed << setprecision(2) << diemCanTim; // Make sure this precision matches what getValue uses
            string tam = convert.str();
            auto start = chrono::high_resolution_clock::now(); // Bắt đầu tính thời gian
            luaChonXuatTenDaoNguocDSLKKep(listKep, "diem", tam);

            auto end = chrono::high_resolution_clock::now();                          // Kết thúc tính thời gian
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Tính thời gian
            int tgianTimKiem = duration.count();
            cout << "\n\t\nTHOI GIAN TIM KIEM : " << tgianTimKiem << endl;

            system("pause");
            break;
        }

        case 10:
        {
            backToMainMenu = true;
            break;
        }

        case 0:
        {
            giaiPhongBoNhoDSLKKep(listKep);
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        }
        }
    }
}

void menuChoMang(SV listMang[], int& soLuongSinhVien)
{
    int luaChon;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH QUAN LY SINH VIEN BANG DANH SACH MANG ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t   1. Xuất danh sách sinh viên";
        cout << "\n\t   2. Tìm kiếm sinh viên";
        cout << "\n\t   3. Thêm mới sinh viên";
        cout << "\n\t   4. Xóa sinh viên";
        cout << "\n\t   5. Liệt kê các sinh viên có điểm thấp nhất";
        cout << "\n\t   6. Liệt kê các sinh viên có điểm cao nhất";
        cout << "\n\t   7. Tính điểm trung bình của lớp";
        cout << "\n\t   8. Sắp xếp danh sách sinh viên";
        cout << "\n\t   10. Quay lai";
        cout << "\n\t   0. Thoát";
        cout << "\n\t======================= END =======================";
        cout << "\n\tVui lòng chọn cách thức lưu trữ(nhập số)";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> luaChon;
        switch (luaChon)
        {
        case 1:
            cout << "\n\t   1. Xuất danh sách sinh viên \n\n";
            inTieuDe();
            xuatSinhVienMang(listMang, soLuongSinhVien);
            system("pause");
            break;
        case 2:
            cout << "\n\t   2. Tìm kiếm sinh viên\n\n";
            timKiemSinhVienMang(listMang, soLuongSinhVien);
            break;
        case 3:
            cout << "\n\t   3. Thêm mới sinh viên\n\n";
            themSinhVienMang(listMang, soLuongSinhVien, soLuongSinhVien);
            cout << "\n\tTHEM THANH CONG !!!\n\n";
            system("pause");
            break;
        case 4:
            cin.ignore();
            cout << "\n\t   4. Xóa sinh viên\n\n";
            xoaSVMang(LIST_MANG, soLuongSinhVien);
            cout << "\n\tXOA THANH CONG !!!\n\n";
            system("pause");
            break;
        case 5:
            lietKeSVDiemCaoNhatMang(LIST_MANG, soLuongSinhVien);
            system("pause");
            break;
        case 6:
            lietKeSVDiemThapNhatMang(LIST_MANG, soLuongSinhVien);
            system("pause");
            break;
        case 7:

            cout << "\n\t-- Diem Trung Binh cua cac sinh vien: " << diemTBMang(LIST_MANG, soLuongSinhVien) << endl;
            system("pause");
            break;
        case 10:
            backToMainMenu = true;
            break;
        case 0:
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        default:
            break;
        }
    }
}

void menuChoDSLKDon(LIST_DON listDon)
{
    int luaChon;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH QUAN LY SINH VIEN BANG DANH SACH LIEN KET DON ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t   1. Xuất danh sách sinh viên";
        cout << "\n\t   2. Tìm kiếm sinh viên";
        cout << "\n\t   3. Thêm mới sinh viên";
        cout << "\n\t   4. Xóa sinh viên";
        cout << "\n\t   5. Liệt kê các sinh viên có điểm thấp nhất";
        cout << "\n\t   6. Liệt kê các sinh viên có điểm cao nhất";
        cout << "\n\t   7. Tính điểm trung bình của lớp";
        cout << "\n\t   8. Sắp xếp danh sách sinh viên";
        cout << "\n\t   10. Quay lai";
        cout << "\n\t   0. Thoát";
        cout << "\n\t======================= END =======================";
        cout << "\n\tVui lòng chọn cách thức lưu trữ(nhập số)";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> luaChon;
        switch (luaChon)
        {
        case 1:
            cout << "\n\t   1. Xuất danh sách sinh viên \n\n";
            inTieuDe();
            xuatDSLKDon(listDon);
            system("pause");
            break;
        case 2:
            cout << "\n\t   2. Tìm kiếm sinh viên\n\n";
            timKiemSinhVienDanhSachLkDon(listDon);
            break;
        case 3:
            cout << "\n\t   3. Thêm mới sinh viên\n\n";
            cout << "- Them sinh vien tiep theo: ";
            themSVVaoCuoiDSLKDon(listDon, nhapThongTinSinhVien());
            cout << "\n\tTHEM THANH CONG !!!\n\n";
            system("pause");
            break;
        case 4:
            cin.ignore();
            cout << "\n\t   4. Xóa sinh viên\n\n";
            xoaSVDSLKDon(listDon);
            cout << "\n\tXOA THANH CONG !!!\n\n";
            system("pause");
            break;
        case 5:
            lietKeSVDiemCaoNhatDSLKDon(listDon);
            system("pause");
            break;
        case 6:
            lietKeSVDiemThapNhatDSLKDon(listDon);
            system("pause");
            break;
        case 7:

            cout << "\n\t-- Diem Trung Binh cua cac sinh vien: " << diemTBDSLKDon(listDon) << endl;
            system("pause");
            break;
        case 10:
            backToMainMenu = true;
            break;
        case 0:
            giaiPhongBoNhoDSLKDon(listDon);
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        default:
            break;
        }
    }
}

void menuChoDSLKVong(LIST_VONG listVong)
{
    int luaChon;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH QUAN LY SINH VIEN BANG DANH SACH LIEN KET VONG ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t   1. Xuất danh sách sinh viên";
        cout << "\n\t   2. Tìm kiếm sinh viên";
        cout << "\n\t   3. Thêm mới sinh viên";
        cout << "\n\t   4. Xóa sinh viên";
        cout << "\n\t   5. Liệt kê các sinh viên có điểm thấp nhất";
        cout << "\n\t   6. Liệt kê các sinh viên có điểm cao nhất";
        cout << "\n\t   7. Tính điểm trung bình của lớp";
        cout << "\n\t   8. Sắp xếp danh sách sinh viên";
        cout << "\n\t   10. Quay lai";
        cout << "\n\t   0. Thoát";
        cout << "\n\t======================= END =======================";
        cout << "\n\tVui lòng chọn cách thức lưu trữ(nhập số)";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> luaChon;
        switch (luaChon)
        {
        case 1:
            inTieuDe();
            xuatDSLKVong(listVong);
            system("pause");
            break;
        case 2:
            timKiemSinhVienDanhSachLkVong(listVong);
            break;
        case 3:
            cout << "- Them sinh vien tiep theo: ";
            themSVVaoCuoiDSLKVong(listVong, nhapThongTinSinhVien());
            cout << "\n\tTHEM THANH CONG !!!\n\n";
            system("pause");
            break;
        case 4:
            cin.ignore();
            cout << "\n\t   4. Xóa sinh viên\n\n";
            xoaSVDSLKVong(listVong);
            cout << "\n\tXOA THANH CONG !!!\n\n";
            system("pause");
            break;
        case 5:
            lietKeSVDiemCaoNhatDSLKVong(listVong);
            system("pause");
            break;
        case 6:
            lietKeSVDiemThapNhatDSLKVong(listVong);
            system("pause");
            break;
        case 7:

            cout << "\n\t-- Diem Trung Binh cua cac sinh vien: " << diemTBDSLKVong(listVong) << endl;
            system("pause");
            break;
        case 10:
            backToMainMenu = true;
            break;
        case 0:
            giaiPhongBoNhoDSLKVong(listVong);
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        default:
            break;
        }
    }
}

void menuChoDSLKKep(LIST_KEP listKep)
{
    int luaChon;
    bool backToMainMenu = false;
    while (!backToMainMenu)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH QUAN LY SINH VIEN BANG DANH SACH LIEN KET KEP ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t   1. Xuất danh sách sinh viên";
        cout << "\n\t   2. Tìm kiếm sinh viên";
        cout << "\n\t   3. Thêm mới sinh viên";
        cout << "\n\t   4. Xóa sinh viên";
        cout << "\n\t   5. Liệt kê các sinh viên có điểm thấp nhất";
        cout << "\n\t   6. Liệt kê các sinh viên có điểm cao nhất";
        cout << "\n\t   7. Tính điểm trung bình của lớp";
        cout << "\n\t   8. Sắp xếp danh sách sinh viên";
        cout << "\n\t   10. Quay lai";
        cout << "\n\t   0. Thoát";
        cout << "\n\t======================= END =======================";
        cout << "\n\tVui lòng chọn cách thức lưu trữ(nhập số)";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> luaChon;
        switch (luaChon)
        {
        case 1:
            inTieuDe();
            xuatDSLKKep(listKep);
            system("pause");
            break;
        case 2:
            timKiemSinhVienDanhSachLKKep(listKep);
            break;
        case 3:
            cout << "- Them sinh vien tiep theo: ";
            themSVVaoCuoiDSLKKep(listKep, nhapThongTinSinhVien());
            cout << "\n\tTHEM THANH CONG !!!\n\n";
            system("pause");
            break;
        case 4:
            cin.ignore();
            cout << "\n\t   4. Xóa sinh viên\n\n";
            xoaSVDSLKKep(listKep);
            cout << "\n\tXOA THANH CONG !!!\n\n";
            system("pause");
            break;
        case 5:
            lietKeSVDiemCaoNhatDSLKKep(listKep);
            system("pause");
            break;
        case 7:

            cout << "\n\t-- Diem Trung Binh cua cac sinh vien: " << diemTBDSLKKep(listKep) << endl;
            system("pause");
            break;
        case 6:
            lietKeSVDiemThapNhatDSLKKep(listKep);
            system("pause");
            break;
        case 10:
            backToMainMenu = true;
            break;
        case 0:
            giaiPhongBoNhoDSLKKep(listKep);
            cout << "\n\t == KET THUC CHUONG TRINH ==\n";
            exit(0);
        default:
            break;
        }
    }
}

static void mainMenu()
{
    int luaChon;
    while (true)
    {
        system("cls");
        cout << "\n\n\t\t=== CHUONG TRINH QUAN LY SINH VIEN ===\n\n";
        cout << "\t======================= MENU =======================";
        cout << "\n\t   1. Lưu trữ bằng danh sách mảng";
        cout << "\n\t   2. Lưu trữ bằng danh sách liên kết đơn";
        cout << "\n\t   3. Lưu trữ bằng danh sách liên kết vòng";
        cout << "\n\t   4. Lưu trữ bằng danh sách liên kết kép";
        cout << "\n\t======================= END =======================";
        cout << "\n\tVui lòng chọn cách thức lưu trữ(nhập số)";
        cout << "\n\n\t - Nhap lua chon: ";
        cin >> luaChon;
        if (luaChon == 1)
        {
            nhapMang();
            menuChoMang(LIST_MANG, soLuong);
        }

        if (luaChon == 2)
        {

            LIST_DON listDon;
            khoitaoDSLKDon(listDon);

            nhapDon(listDon);
            menuChoDSLKDon(listDon);
        }

        if (luaChon == 3)
        {

            LIST_VONG listVong;
            khoiTaoDSLKVong(listVong);

            nhapVong(listVong);
            menuChoDSLKVong(listVong);
        }

        if (luaChon == 4)
        {

            LIST_KEP listKep;
            khoiTaoDSLKKep(listKep);

            nhapKep(listKep);
            menuChoDSLKKep(listKep);
        }
    }
}

int main()
{
    mainMenu();

    return 0;
}

// Comand nday