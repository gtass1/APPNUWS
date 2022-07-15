
inline unsigned long IDdoUID(int *id) {
    unsigned long uid=id[2]*1e+10 + id[1]*1e+5 + id[0];
    return uid;
};

inline void UIDtoID(const unsigned long &uid, int *id) {
    id[0]=uid % ((unsigned long)1e+5);
    id[1]=( uid % ((unsigned long)1e+10) ) / ((unsigned long)1e+5);
    id[2]= uid / ((unsigned long)1e+10);
};
