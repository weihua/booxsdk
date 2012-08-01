#ifndef ONYX_DATA_TAGS_H_
#define ONYX_DATA_TAGS_H_

/// Define all tags here.

static const QString TAG_NAME  = "name";
static const QString TAG_TITLE = "title";
static const QString TAG_SUB_TITLE = "sub_title";
static const QString TAG_AUTHOR = "author";
static const QString TAG_ISBN = "isbn";
static const QString TAG_COVER = "cover";
static const QString TAG_TYPE = "type";
static const QString TAG_TAGS = "tags";
static const QString TAG_URL = "url";
static const QString TAG_ID  = "id";
static const QString TAG_FS_NAME_FILTERS = "fs_n_filter";
static const QString TAG_FS_SIZE = "fs_size";
static const QString TAG_LAST_ACCESS = "l_access";
static const QString TAG_READ_COUNT = "r_count";
static const QString TAG_CUSTOM = "custom";

static const QString TAG_BRANCH = "branch"; // for opds, it is identical to catalog
static const QString TAG_ENTRY = "entry";
static const QString TAG_SUFFIX = "suffix";
static const QString TAG_BOOKINFO = "bookinfo";

static const QString FILE_TAG = "file";
static const QString DIR_TAG = "dir";
static const QString WEBSITE_TAG ="website";
static const QString APPLICATION_TAG = "application";
static const QString TAG_USER_DEFINED_PREFIX = "user_defined_";

static const QString TAG_FONT_FAMILY = "fontfamily";
static const QString TAG_FONT_SIZE = "fontsize";
static const QString TAG_CHECKED = "checked";
static const QString TAG_ALIGN = "align";
static const QString TAG_DISABLED = "disabled";

static const QString TAG_MENU_TYPE = "menu_type"; // for keyboard
static const QString TAG_SPECIAL_KEY = "special_key";
static const QString TAG_SPECIAL_KEY_TEXT = "special_key_text";

static const QString TAG_IS_PASSWD = "is_password"; // denote the data is password or not.
static const QString TAG_DEFAULT_VALUE = "default_value";

static const QString TAG_MARKER = "marker";

static const QString TAG_ICON = "icon";

static const QString TAG_ATTRIBUTE = "attribute";

static const QString TAG_CDL_BOOK_TYPE = "cdl_book_type";
static const QString TAG_CDL_BOOK_ENCRYPT_TYPE = "cdl_book_encrypt_type";
static const QString TAG_CDL_BOOK_PRICE = "cdl_book_price";
static const QString TAG_CDL_BOOK_RATING = "cdl_book_rating";
static const QString TAG_CDL_BOOK_LOCAL_UPDATE_TIME = "cdl_book_local_update_time";
static const QString TAG_CDL_BOOK_SERVER_UPDATE_TIME = "cdl_book_server_update_time";
static const QString TAG_CDL_BOOK_ID = "cdl_book_id";
static const QString TAG_COVER_BIG ="big_cover";
#endif
