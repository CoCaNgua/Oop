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
                if (i == soBuoc) {
                    if (banCo[viTriCheck]->mau == ngua.mau) return true;
                } else {
                    return true;
                }
            }
        }
        return false;
    }

    void xuLyDaNgua(QuanCo& nguaTa, int viTriMoi, NguoiChoi& nguoiChoiHienTai) {
        if (banCo[viTriMoi] != nullptr && banCo[viTriMoi]->mau != nguaTa.mau) {
            cout << "Da ngua! Quan cua doi khac phai ve chuong.\n";
            
            banCo[viTriMoi]->trangThai = TRONG_CHUONG;
            banCo[viTriMoi]->viTri = -1;
            banCo[viTriMoi]->quangDuongDaDi = 0;
            
            nguoiChoiHienTai.duocThemLuot = true;
        }
    }

    void thucHienDiChuyen(NguoiChoi& p, int indexNgua, int diemXucXac) {
        QuanCo& ngua = p.danhSachNgua[indexNgua];

        if (ngua.trangThai == TRONG_CHUONG) {
            if (diemXucXac == 6 || diemXucXac == 1) { 
                int batDau = ngua.layViTriBatDau();
                if (banCo[batDau] == nullptr || banCo[batDau]->mau != ngua.mau) {
                    ngua.trangThai = TREN_BAN_CO;
                    ngua.viTri = batDau;
                    banCo[batDau] = &ngua;
                }
            }
            return;
        }

        if (ngua.trangThai == TREN_BAN_CO) {
            if (biCanDuong(ngua, diemXucXac)) {
                cout << "Khong the di chuyen do co vat can!\n";
                return;
            }

            if (ngua.quangDuongDaDi + diemXucXac >= 48) {
                ngua.trangThai = LEN_DOC;
                ngua.viTri = (ngua.quangDuongDaDi + diemXucXac) - 48 + 1;
                banCo[ngua.viTri] = nullptr; 
                return;
            }

            banCo[ngua.viTri] = nullptr;
            int viTriMoi = (ngua.viTri + diemXucXac) % 48;
            
            xuLyDaNgua(ngua, viTriMoi, p);
            
            ngua.viTri = viTriMoi;
            ngua.quangDuongDaDi += diemXucXac;
            banCo[viTriMoi] = &ngua;
        }

        else if (ngua.trangThai == LEN_DOC) {
            if (diemXucXac == ngua.viTri + 1) {
                ngua.viTri = diemXucXac;
                cout << "Ngua da len bac: " << ngua.viTri << endl;
                
                if (ngua.viTri == 6) {
                    ngua.trangThai = VE_DICH;
                    p.duocThemLuot = true; 
                    cout << "Chuc mung! Ngua da ve dich.\n";
                }
            }
        }
    }
};
