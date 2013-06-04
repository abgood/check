/* 独立信息 */
create table indepe_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    name varchar(100) not null default '',
    site varchar(20) not null default '',
    telecom_ip varchar(20) not null default '',
    unicom_ip varchar(20) not null default '',
    site_flag integer not null default 0
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/* 共同信息 */
create table common_info (
    `Id` int(20) NOT NULL AUTO_INCREMENT primary key COMMENT '编号',
    site_name varchar(15) not null default '',
    domain varchar(30) not null default '',
    bind_flag boolean not null default 0
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
