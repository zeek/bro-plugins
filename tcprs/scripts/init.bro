
const tcprs_enabled = F &redef;

type rtx_stats: record {
        unknown: int;
        hs: int;
        ss: int;
        ca: int;
        fast: int;
        close: int;
        windowlimited: int;
        steady: int;
        zerowindow: int;
        retrans: int;
        idle: int;
};

