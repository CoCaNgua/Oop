#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

enum TrangThai {
    TRONG_CHUONG,
    TREN_BAN_CO,
    LEN_DOC,
    VE_DICH
};

enum MauSac {
    XANH_DUONG = 0,
    DO = 1,
    XANH_LA = 2,
    VANG = 3
};

class QuanCo {
public:
    MauSac mau;
    int id;
    int viTri;
    int quangDuongDaDi;
    TrangThai trangThai;

    QuanCo(MauSac m, int i) {
        mau = m;
        id = i;
        viTri = -1;
        quangDuongDaDi = 0;
        trangThai = TRONG_CHUONG;
    }

    int layViTriBatDau() const {
        switch (mau) {
            case XANH_DUONG: return 0;
            case DO: return 12;
            case XANH_LA: return 24;
            case VANG: return 36;
        }
        return 0;
    }
};

class DieuKhienTroChoi;

class ChienThuat {
public:
    virtual int chonNgua(
        class NguoiChoi& p,
        DieuKhienTroChoi& game,
        int diemXucXac,
        vector<int>& nguaHopLe
    ) = 0;

    virtual string ten() = 0;

    virtual ~ChienThuat() {}
};

class NguoiChoi {
public:
    MauSac mau;
    vector<QuanCo> danhSachNgua;
    ChienThuat* chienThuat;
    bool duocThemLuot;
    int soQuanVeDich;

    NguoiChoi(MauSac m, ChienThuat* ct) {
        mau = m;
        chienThuat = ct;
        duocThemLuot = false;
        soQuanVeDich = 0;

        for (int i = 0; i < 4; i++) {
            danhSachNgua.push_back(QuanCo(m, i));
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

    QuanCo* layQuanTai(int viTri) {
        return banCo[viTri];
    }

    bool biCanDuong(QuanCo& ngua, int soBuoc) {
        if (ngua.trangThai != TREN_BAN_CO)
            return false;

        for (int i = 1; i <= soBuoc; i++) {

            int viTriCheck = (ngua.viTri + i) % 48;

            if (banCo[viTriCheck] != nullptr) {

                if (i == soBuoc &&
                    banCo[viTriCheck]->mau == ngua.mau)
                    return true;

                if (i < soBuoc)
                    return true;
            }
        }

        return false;
    }

    void xuLyDaNgua(
        QuanCo& nguaTa,
        int viTriMoi,
        NguoiChoi& nguoiChoi
    ) {
        if (banCo[viTriMoi] != nullptr &&
            banCo[viTriMoi]->mau != nguaTa.mau)
        {
            QuanCo* biDa = banCo[viTriMoi];

            biDa->trangThai = TRONG_CHUONG;
            biDa->viTri = -1;
            biDa->quangDuongDaDi = 0;

            nguoiChoi.duocThemLuot = true;
        }
    }

    vector<int> layDanhSachHopLe(
        NguoiChoi& p,
        int diemXucXac
    ) {
        vector<int> ds;

        for (int i = 0; i < 4; i++) {

            QuanCo& ngua = p.danhSachNgua[i];

            if (ngua.trangThai == VE_DICH)
                continue;

            if (ngua.trangThai == TRONG_CHUONG) {

                if (diemXucXac == 6) {

                    int start = ngua.layViTriBatDau();

                    if (banCo[start] == nullptr ||
                        banCo[start]->mau != ngua.mau)
                    {
                        ds.push_back(i);
                    }
                }
            }

            else if (ngua.trangThai == TREN_BAN_CO) {

                if (!biCanDuong(ngua, diemXucXac)) {
                    ds.push_back(i);
                }
            }

            else if (ngua.trangThai == LEN_DOC) {

                if (diemXucXac == ngua.viTri + 1) {
                    ds.push_back(i);
                }
            }
        }

        return ds;
    }

    void diChuyen(
        NguoiChoi& p,
        int indexNgua,
        int diemXucXac
    ) {
        QuanCo& ngua = p.danhSachNgua[indexNgua];

        if (ngua.trangThai == TRONG_CHUONG) {

            int start = ngua.layViTriBatDau();

            xuLyDaNgua(ngua, start, p);

            ngua.trangThai = TREN_BAN_CO;
            ngua.viTri = start;
            ngua.quangDuongDaDi = 0;

            banCo[start] = &ngua;

            return;
        }

        if (ngua.trangThai == TREN_BAN_CO) {

            banCo[ngua.viTri] = nullptr;

            if (ngua.quangDuongDaDi + diemXucXac > 48) {

                banCo[ngua.viTri] = &ngua;
                return;
            }

            if (ngua.quangDuongDaDi + diemXucXac == 48) {

                ngua.trangThai = LEN_DOC;
                ngua.viTri = 0;

                return;
            }

            int viTriMoi =
                (ngua.viTri + diemXucXac) % 48;

            xuLyDaNgua(ngua, viTriMoi, p);

            ngua.viTri = viTriMoi;
            ngua.quangDuongDaDi += diemXucXac;

            banCo[viTriMoi] = &ngua;

            return;
        }

        if (ngua.trangThai == LEN_DOC) {

            if (diemXucXac == ngua.viTri + 1) {

                ngua.viTri++;

                if (ngua.viTri == 6) {

                    ngua.trangThai = VE_DICH;

                    p.soQuanVeDich++;

                    p.duocThemLuot = true;
                }
            }
        }
    }
};

class HungHang : public ChienThuat {
public:

    string ten() override {
        return "Hung Hang";
    }

    int chonNgua(
        NguoiChoi& p,
        DieuKhienTroChoi& game,
        int diemXucXac,
        vector<int>& ds
    ) override {

        for (int idx : ds) {

            QuanCo& n = p.danhSachNgua[idx];

            if (n.trangThai == TREN_BAN_CO) {

                int vMoi =
                    (n.viTri + diemXucXac) % 48;

                QuanCo* q = game.layQuanTai(vMoi);

                if (q != nullptr &&
                    q->mau != n.mau)
                {
                    return idx;
                }
            }
        }

        int best = ds[0];

        for (int idx : ds) {

            if (p.danhSachNgua[idx].quangDuongDaDi >
                p.danhSachNgua[best].quangDuongDaDi)
            {
                best = idx;
            }
        }

        return best;
    }
};

class UuTienVeDich : public ChienThuat {
public:

    string ten() override {
        return "Uu Tien Ve Dich";
    }

    int chonNgua(
        NguoiChoi& p,
        DieuKhienTroChoi& game,
        int diemXucXac,
        vector<int>& ds
    ) override {

        int best = ds[0];

        for (int idx : ds) {

            if (p.danhSachNgua[idx].quangDuongDaDi >
                p.danhSachNgua[best].quangDuongDaDi)
            {
                best = idx;
            }
        }

        return best;
    }
};

class DanQuan : public ChienThuat {
public:

    string ten() override {
        return "Dan Quan";
    }

    int chonNgua(
        NguoiChoi& p,
        DieuKhienTroChoi& game,
        int diemXucXac,
        vector<int>& ds
    ) override {

        if (diemXucXac == 6) {

            for (int idx : ds) {

                if (p.danhSachNgua[idx].trangThai
                    == TRONG_CHUONG)
                {
                    return idx;
                }
            }
        }

        sort(ds.begin(), ds.end(),
            [&](int a, int b)
            {
                return p.danhSachNgua[a].quangDuongDaDi
                    <
                    p.danhSachNgua[b].quangDuongDaDi;
            });

        return ds[0];
    }
};

class NgauNhien : public ChienThuat {
public:

    string ten() override {
        return "Ngau Nhien";
    }

    int chonNgua(
        NguoiChoi& p,
        DieuKhienTroChoi& game,
        int diemXucXac,
        vector<int>& ds
    ) override {

        return ds[rand() % ds.size()];
    }
};

int main() {

    srand(time(0));

    int n;
    cin >> n;

    int win1 = 0;
    int win2 = 0;
    int win3 = 0;
    int win4 = 0;

    HungHang ct1;
    UuTienVeDich ct2;
    DanQuan ct3;
    NgauNhien ct4;

    while (n--) {

        DieuKhienTroChoi game;

        vector<NguoiChoi> players;

        players.push_back(
            NguoiChoi(XANH_DUONG, &ct1));

        players.push_back(
            NguoiChoi(DO, &ct2));

        players.push_back(
            NguoiChoi(XANH_LA, &ct3));

        players.push_back(
            NguoiChoi(VANG, &ct4));

        bool gameRunning = true;

        while (gameRunning) {

            for (auto& p : players) {

                bool tiepTuc = true;

                while (tiepTuc) {

                    p.duocThemLuot = false;

                    int xucXac =
                        rand() % 6 + 1;

                    if (xucXac == 6)
                        p.duocThemLuot = true;

                    vector<int> dsHopLe =
                        game.layDanhSachHopLe(
                            p,
                            xucXac
                        );

                    if (!dsHopLe.empty()) {

                        int idx =
                            p.chienThuat->chonNgua(
                                p,
                                game,
                                xucXac,
                                dsHopLe
                            );

                        game.diChuyen(
                            p,
                            idx,
                            xucXac
                        );
                    }

                    if (p.soQuanVeDich == 4) {

                        if (p.mau == XANH_DUONG)
                            win1++;

                        else if (p.mau == DO)
                            win2++;

                        else if (p.mau == XANH_LA)
                            win3++;

                        else
                            win4++;

                        gameRunning = false;
                        break;
                    }

                    tiepTuc = p.duocThemLuot;
                }

                if (!gameRunning)
                    break;
            }
        }
    }

    cout << "Hung Hang: " << win1 << endl;
    cout << "Uu Tien Ve Dich: " << win2 << endl;
    cout << "Dan Quan: " << win3 << endl;
    cout << "Ngau Nhien: " << win4 << endl;

    return 0;
}
