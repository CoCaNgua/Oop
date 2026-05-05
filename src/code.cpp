#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

enum TrangThai { TRONG_CHUONG, TREN_BAN_CO, LEN_DOC, VE_DICH };
enum MauSac { XANH_DUONG = 0, DO = 1, XANH_LA = 2, VANG = 3 };

struct QuanCo {
    MauSac mau;
    int id;
    int viTri;
    int quangDuongDaDi;
    TrangThai trangThai;

    QuanCo(MauSac m, int i) : mau(m), id(i), viTri(-1), quangDuongDaDi(0), trangThai(TRONG_CHUONG) {}

    int layViTriBatDau() {
        switch (mau) {
            case XANH_DUONG: return 0;
            case DO: return 12;
            case XANH_LA: return 24;
            case VANG: return 36;
            default: return 0;
        }
    }
};

class NguoiChoi {
public:
    MauSac mau;
    string tenChienThuat;
    int loaiChienThuat;
    vector<QuanCo> danhSachNgua;
    bool duocThemLuot;
    int soQuanVeDich;

    NguoiChoi(MauSac m, int ct) : mau(m), loaiChienThuat(ct), duocThemLuot(false), soQuanVeDich(0) {
        for (int i = 0; i < 4; i++) {
            danhSachNgua.push_back(QuanCo(m, i));
        }
        switch (ct) {
            case 1: tenChienThuat = "Hung Hang"; break;
            case 2: tenChienThuat = "Uu Tien Ve Dich"; break;
            case 3: tenChienThuat = "Dao Quan - Rao Chan"; break;
            default: tenChienThuat = "Ngau Nhien"; break;
        }
    }
};

class DieuKhienTroChoi {
private:
    QuanCo* banCo[48];

public:
    DieuKhienTroChoi() {
        for (int i = 0; i < 48; i++) banCo[i] = nullptr;
    }


    bool biCanDuong(QuanCo& ngua, int soBuoc) {
        if (ngua.trangThai != TREN_BAN_CO) return false;

        for (int i = 1; i <= soBuoc; i++) {
            int viTriCheck = (ngua.viTri + i) % 48;
            if (banCo[viTriCheck] != nullptr) {
                if (i == soBuoc && banCo[viTriCheck]->mau == ngua.mau) return true;
                if (i < soBuoc) return true;
            }
        }
        return false;
    }

    void xuLyDaNgua(QuanCo& nguaTa, int viTriMoi, NguoiChoi& nguoiChoiHienTai) {
        if (banCo[viTriMoi] != nullptr && banCo[viTriMoi]->mau != nguaTa.mau) {
            cout << "  -> DA NGUA! Quan cua doi thu mau " << banCo[viTriMoi]->mau << " phai ve chuong.\n";
            banCo[viTriMoi]->trangThai = TRONG_CHUONG;
            banCo[viTriMoi]->viTri = -1;
            banCo[viTriMoi]->quangDuongDaDi = 0;
            nguoiChoiHienTai.duocThemLuot = true; 
        }
    }

    void thucHienDiChuyen(NguoiChoi& p, int indexNgua, int diemXucXac) {
        QuanCo& ngua = p.danhSachNgua[indexNgua];

        if (ngua.trangThai == TRONG_CHUONG) {
            int batDau = ngua.layViTriBatDau();
            ngua.trangThai = TREN_BAN_CO;
            ngua.viTri = batDau;
            xuLyDaNgua(ngua, batDau, p);
            banCo[batDau] = &ngua;
            cout << "  -> Ngua " << indexNgua << " xuat chuong!\n";
        }
        else if (ngua.trangThai == TREN_BAN_CO) {
            banCo[ngua.viTri] = nullptr;
if (ngua.quangDuongDaDi + diemXucXac >= 48) {
                ngua.trangThai = LEN_DOC;
                ngua.viTri = (ngua.quangDuongDaDi + diemXucXac) - 48 + 1;
                cout << "  -> Ngua " << indexNgua << " bat dau len doc tai bac " << ngua.viTri << "\n";
            } else {
                int viTriMoi = (ngua.viTri + diemXucXac) % 48;
                xuLyDaNgua(ngua, viTriMoi, p);
                ngua.viTri = viTriMoi;
                ngua.quangDuongDaDi += diemXucXac;
                banCo[viTriMoi] = &ngua;
                cout << "  -> Ngua " << indexNgua << " di den o " << viTriMoi << "\n";
            }
        }
        else if (ngua.trangThai == LEN_DOC) {
            ngua.viTri = diemXucXac;
            cout << "  -> Ngua " << indexNgua << " len bac " << ngua.viTri << "\n";
            if (ngua.viTri == 6) {
                ngua.trangThai = VE_DICH;
                p.soQuanVeDich++;
                p.duocThemLuot = true; 
                cout << "  -> CHUC MUNG! Ngua " << indexNgua << " da ve dich.\n";
            }
        }
    }

    int chonNgua(NguoiChoi& p, int diemXucXac) {
        vector<int> nguaHopLe;
        for (int i = 0; i < 4; i++) {
            QuanCo& ngua = p.danhSachNgua[i];
            if (ngua.trangThai == VE_DICH) continue;

            if (ngua.trangThai == TRONG_CHUONG) {
                if (diemXucXac == 6) {
                    int batDau = ngua.layViTriBatDau();
                    if (banCo[batDau] == nullptr || banCo[batDau]->mau != ngua.mau) nguaHopLe.push_back(i);
                }
            } else if (ngua.trangThai == TREN_BAN_CO) {
                if (!biCanDuong(ngua, diemXucXac)) nguaHopLe.push_back(i);
            } else if (ngua.trangThai == LEN_DOC) {
                if (diemXucXac == ngua.viTri + 1) nguaHopLe.push_back(i);
            }
        }

        if (nguaHopLe.empty()) return -1;

        int selected = nguaHopLe[0];
        switch (p.loaiChienThuat) {
            case 1:
                for (int idx : nguaHopLe) {
                    QuanCo& n = p.danhSachNgua[idx];
                    if (n.trangThai == TREN_BAN_CO) {
                        int vMoi = (n.viTri + diemXucXac) % 48;
                        if (banCo[vMoi] != nullptr && banCo[vMoi]->mau != n.mau) return idx;
                    }
                }
                break;
            case 2:
                for (int idx : nguaHopLe) {
                    if (p.danhSachNgua[idx].quangDuongDaDi > p.danhSachNgua[selected].quangDuongDaDi) selected = idx;
                }
                return selected;
            case 3:
                for (int idx : nguaHopLe) {
                    if (p.danhSachNgua[idx].quangDuongDaDi < p.danhSachNgua[selected].quangDuongDaDi) selected = idx;
                }
                return selected;
        }
        return nguaHopLe[rand() % nguaHopLe.size()];
    }
};


int main() {
    srand(time(0));
    DieuKhienTroChoi game;
    vector<NguoiChoi> players;
players.push_back(NguoiChoi(XANH_DUONG, 1));
    players.push_back(NguoiChoi(DO, 2));
    players.push_back(NguoiChoi(XANH_LA, 3));
    players.push_back(NguoiChoi(VANG, 4));

    bool keepPlaying = true;
    int round = 1;

    while (keepPlaying) {
        cout << "--- VONG " << round++ << " ---\n";
        for (auto& p : players) {
            bool turnContinue = true;
            while (turnContinue) {
                p.duocThemLuot = false;
                int xucXac = rand() % 6 + 1;
                if (xucXac == 6) p.duocThemLuot = true;

                cout << "Nguoi choi " << p.mau << " (" << p.tenChienThuat << ") tung xuc xac: " << xucXac << endl;

                int indexNgua = game.chonNgua(p, xucXac);
                if (indexNgua != -1) {
                    game.thucHienDiChuyen(p, indexNgua, xucXac);
                } else {
                    cout << "  -> Khong co nuoc di hop le.\n";
                }

                if (p.soQuanVeDich == 4) {
                    cout << "\n==============================\n";
                    cout << "NGUOI CHOI " << p.mau << " CHIEN THANG!\n";
                    keepPlaying = false;
                    turnContinue = false;
                    break;
                }

                turnContinue = p.duocThemLuot;
                if (turnContinue) cout << "  -> Duoc them luot!\n";
            }
            if (!keepPlaying) break;
        }
        if (round > 500) break;
    }

    return 0;
}
