#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

enum TrangThai { TRONG_CHUONG, TREN_BAN_CO, LEN_DOC, VE_DICH };
enum MauSac { XANH_DUONG = 0, DO = 1, XANH_LA = 2, VANG = 3 };

class QuanCo {
public:
    MauSac mau;
    int id;
    int viTri;
    int quangDuongDaDi;
    TrangThai trangThai;

    QuanCo(MauSac m, int i) : mau(m), id(i), viTri(-1), quangDuongDaDi(0), trangThai(TRONG_CHUONG) {}

    int layViTriBatDau() const {
        if (mau == XANH_DUONG) return 0;
        if (mau == DO) return 12;
        if (mau == XANH_LA) return 24;
        return 36;
    }
};

class DieuKhienTroChoi;

class ChienThuat {
public:
    virtual int chonNgua(class NguoiChoi& p, DieuKhienTroChoi& game, int dice, int* ds, int size) = 0;
    virtual string ten() = 0;
    virtual ~ChienThuat() {}
};

class NguoiChoi {
public:
    MauSac mau;
    QuanCo* danhSachNgua[4];
    ChienThuat* chienThuat;
    bool duocThemLuot;
    int soQuanVeDich;

    NguoiChoi(MauSac m, ChienThuat* ct) : mau(m), chienThuat(ct), duocThemLuot(false), soQuanVeDich(0) {
        for (int i = 0; i < 4; i++) {
            danhSachNgua[i] = new QuanCo(m, i);
        }
    }

    ~NguoiChoi() {
        for (int i = 0; i < 4; i++) {
            delete danhSachNgua[i];
        }
    }
};

class DieuKhienTroChoi {
private:
    QuanCo* banCo[48];

public:
    DieuKhienTroChoi() {
        for (int i = 0; i < 48; i++) {
            banCo[i] = nullptr;
        }
    }

    QuanCo* layQuanTai(int vt) {
        if (vt >= 0 && vt < 48) return banCo[vt];
        return nullptr;
    }

    bool biCanDuong(QuanCo& ngua, int soBuoc) {
        if (ngua.trangThai != TREN_BAN_CO) return false;

        for (int i = 1; i <= soBuoc; i++) {
            int vtCheck = (ngua.viTri + i) % 48;
            if (banCo[vtCheck] != nullptr) {
                if (i == soBuoc && banCo[vtCheck]->mau == ngua.mau) return true;
                if (i < soBuoc) return true;
            }
        }
        return false;
    }

    void xuLyDaNgua(QuanCo& ta, int vtMoi, NguoiChoi& p) {
        if (vtMoi >= 0 && vtMoi < 48 && banCo[vtMoi] != nullptr && banCo[vtMoi]->mau != ta.mau) {
            QuanCo* biDa = banCo[vtMoi];
            biDa->trangThai = TRONG_CHUONG;
            biDa->viTri = -1;
            biDa->quangDuongDaDi = 0;
            p.duocThemLuot = true;
        }
    }

    int layDanhSachHopLe(NguoiChoi& p, int dice, int* ds) {
        int count = 0;
        for (int i = 0; i < 4; i++) {
            QuanCo& n = *p.danhSachNgua[i];

            if (n.trangThai == VE_DICH) continue;

            if (n.trangThai == TRONG_CHUONG) {
                if (dice == 6) {
                    int start = n.layViTriBatDau();
                    if (banCo[start] == nullptr || banCo[start]->mau != n.mau) {
                        ds[count++] = i;
                    }
                }
            } else {
                int qdMoi = n.quangDuongDaDi + dice;
                if (qdMoi <= 54) {
                    if (n.trangThai == TREN_BAN_CO) {
                        if (!biCanDuong(n, dice)) ds[count++] = i;
                    } else {
                        ds[count++] = i;
                    }
                }
            }
        }
        return count;
    }

    void diChuyen(NguoiChoi& p, int idx, int dice) {
        QuanCo& n = *p.danhSachNgua[idx];

        if (n.trangThai == TRONG_CHUONG) {
            int start = n.layViTriBatDau();
            xuLyDaNgua(n, start, p);

            n.trangThai = TREN_BAN_CO;
            n.viTri = start;
            n.quangDuongDaDi = 1;
            banCo[start] = &n;
            return;
        }

        if (n.trangThai == TREN_BAN_CO) {
            banCo[n.viTri] = nullptr;
        }

        int qdMoi = n.quangDuongDaDi + dice;
        n.quangDuongDaDi = qdMoi;

        if (qdMoi < 48) {
            int vtMoi = (n.viTri + dice) % 48;
            xuLyDaNgua(n, vtMoi, p);
            n.viTri = vtMoi;
            banCo[vtMoi] = &n;
        }
        else if (qdMoi < 54) {
            n.trangThai = LEN_DOC;
            n.viTri = qdMoi - 47;
        }
        else if (qdMoi == 54) {
            n.trangThai = VE_DICH;
            n.viTri = 100;
            p.soQuanVeDich++;
            p.duocThemLuot = true;
        }
    }
};


class HungHang : public ChienThuat {
public:
    string ten() override { return "Hung Hang"; }

    int chonNgua(NguoiChoi& p, DieuKhienTroChoi& game, int dice, int* ds, int size) override {
        for (int i = 0; i < size; i++) {
            QuanCo& n = *p.danhSachNgua[ds[i]];
            if (n.trangThai == TREN_BAN_CO) {
                int vtMoi = (n.viTri + dice) % 48;
                QuanCo* q = game.layQuanTai(vtMoi);
                if (q != nullptr && q->mau != n.mau) return ds[i];
            }
        }

        int best = ds[0];
        for (int i = 1; i < size; i++) {
            if (p.danhSachNgua[ds[i]]->quangDuongDaDi > p.danhSachNgua[best]->quangDuongDaDi) {
                best = ds[i];
            }
        }
        return best;
    }
};

class UuTienVeDich : public ChienThuat {
public:
    string ten() override { return "Uu Tien Ve Dich"; }

    int chonNgua(NguoiChoi& p, DieuKhienTroChoi& game, int dice, int* ds, int size) override {
        int best = ds[0];
        for (int i = 1; i < size; i++) {
            if (p.danhSachNgua[ds[i]]->quangDuongDaDi > p.danhSachNgua[best]->quangDuongDaDi) {
                best = ds[i];
            }
        }
        return best;
    }
};

class DanQuan : public ChienThuat {
public:
    string ten() override { return "Dan Quan"; }

    int chonNgua(NguoiChoi& p, DieuKhienTroChoi& game, int dice, int* ds, int size) override {
        if (dice == 6) {
            for (int i = 0; i < size; i++) {
                if (p.danhSachNgua[ds[i]]->trangThai == TRONG_CHUONG) return ds[i];
            }
        }

        int best = ds[0];
        for (int i = 1; i < size; i++) {
            if (p.danhSachNgua[ds[i]]->quangDuongDaDi < p.danhSachNgua[best]->quangDuongDaDi) {
                best = ds[i];
            }
        }
        return best;
    }
};

class NgauNhien : public ChienThuat {
public:
    string ten() override { return "Ngau Nhien"; }

    int chonNgua(NguoiChoi& p, DieuKhienTroChoi& game, int dice, int* ds, int size) override {
        return ds[rand() % size];
    }
};

int main() {
    srand(time(0));
    int n;

    cin >> n;

    int wins[4] = {0, 0, 0, 0};

    HungHang ct1;
    UuTienVeDich ct2;
    DanQuan ct3;
    NgauNhien ct4;

    for (int v = 0; v < n; v++) {
        DieuKhienTroChoi game;
        NguoiChoi* players[4];

        players[0] = new NguoiChoi(XANH_DUONG, &ct1);
        players[1] = new NguoiChoi(DO, &ct2);
        players[2] = new NguoiChoi(XANH_LA, &ct3);
        players[3] = new NguoiChoi(VANG, &ct4);

        bool running = true;

        while (running) {
            for (int j = 0; j < 4; j++) {
                bool tiepTuc = true;

                while (tiepTuc) {
                    players[j]->duocThemLuot = false;
                    int dice = rand() % 6 + 1;

                    if (dice == 6) {
                        players[j]->duocThemLuot = true;
                    }

                    int ds[4];
                    int size = game.layDanhSachHopLe(*players[j], dice, ds);

                    if (size > 0) {
                        int choice = players[j]->chienThuat->chonNgua(*players[j], game, dice, ds, size);
                        game.diChuyen(*players[j], choice, dice);
                    }
                    if (players[j]->soQuanVeDich == 4) {
                        wins[j]++;
                        running = false;
                        break;
                    }
                    tiepTuc = players[j]->duocThemLuot;
                }
                if (!running) break;
            }
        }

        for (int j = 0; j < 4; j++) {
            delete players[j];
        }
    }

    cout << "Hung Hang: " << wins[0] << endl;
    cout << "Uu Tien Ve Dich: " << wins[1] << endl;
    cout << "Dan Quan: " << wins[2] << endl;
    cout << "Ngau Nhien: " << wins[3] << endl;

    return 0;
}
