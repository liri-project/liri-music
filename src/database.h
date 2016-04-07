#ifndef LIRI_MUSIC_DATABASE_H
#define LIRI_MUSIC_DATABASE_H

#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor.hpp>

namespace database
{
    namespace queries {
        struct create;
        struct find;
        struct find_all;
        struct exists;
        struct insert;
    }

    namespace detail {
        template<typename Tag>
        struct get {};

        template<typename Tag>
        struct name {};

        template<typename Tag>
        struct set{};

        template<typename T, typename Tag>
        struct query {};

        template<typename T>
        struct mapper {};

        template<typename T>
        struct table_name {};

        template<typename T>
        struct field_type {};

        template<typename T>
        struct result_type {};

        template<typename T>
        struct field_name {};

        template<typename T>
        struct query_binder {
            query_binder(QSqlQuery& q, const T& item)
                : query(q), index(0), item(item) {
            }

            template<typename U>
            QVariant operator()(U&) {
                QVariant result = detail::get<U>::apply(item);
                query.bindValue(index, result);
                ++index;
                return result;
            }
        private:
            int index;
            QSqlQuery& query;
            const T& item;
        };

        template<typename T>
        struct result_binder {
            result_binder(const QSqlQuery& q, T& item)
                : query(q), item(item), index(0) {
            }

            template<typename U>
            void operator()(U) {
                detail::set<typename U::first>::apply(item,
                                                      query.value(index).value<typename U::second>());
                ++index;
            }
        private:
            int index;
            T& item;
            const QSqlQuery& query;
        };
    }

    template<typename T>
    struct Table {
        static constexpr const char* name =
                detail::name<T>::value;
        using find = detail::query<T, queries::find>;
        using create = detail::query<T, queries::create>;
        using find_all = detail::query<T, queries::find_all>;
        using exists = detail::query<T, queries::exists>;
        using insert = detail::query<T, queries::insert>;
        using mapper = typename detail::mapper<T>::value;
    };

    template<typename T>
    T map(QSqlQuery& q) {
        T item;
        detail::result_binder<T> binder (q, item);
        boost::mpl::for_each<typename Table<T>::mapper>(binder);
        return item;
    }

    template<typename T>
    void makeTable(QSqlDatabase& db) {
        QSqlQuery create { Table<T>::create::value, db };
    }

    template<typename T>
    auto findAll(QSqlDatabase& db)
        -> QList<T> {
        QList<T> items;
        QSqlQuery q { Table<T>::find_all::value, db };
        while(q.next()) {
            items.push_back(map<T>(q));
        }
        return items;
    }

    template<typename T>
    QList<T> find(QSqlDatabase& db, const T& item) {
        QList<T> items;
        QSqlQuery q { db };
        q.prepare(Table<T>::find::value);
        detail::query_binder<T> binder (q, item);
        boost::mpl::for_each<typename Table<T>::find::params>(binder);

        q.exec();
        while(q.next()) {
            items.push_back(map<T>(q));
        }
        return items;
    }

    template<typename T>
    bool exists(QSqlDatabase& db, T item) {
        QSqlQuery q;
        q.prepare(Table<T>::exists::value);
        detail::query_binder<T> binder { q, item };
        boost::mpl::for_each<typename Table<T>::exists::params>(binder);
        q.exec();
        q.next();
        if(q.value(0).toInt() != 0)
            return true;
        return false;
    }

    template<typename T>
    void insert(QSqlDatabase& db, const T& item) {
        QSqlQuery q { db };
        q.prepare(Table<T>::insert::value);
        detail::query_binder<T> binder { q, item };
        boost::mpl::for_each<typename Table<T>::insert::params>(binder);
        q.exec();
    }

    template<typename T>
    QList<typename database::detail::result_type<T>::value> find_by(QSqlDatabase& db, const typename database::detail::field_type<T>::value & value) {
        QSqlQuery q { db };
        q.prepare(QString("SELECT * FROM ") + database::detail::table_name<T>::value + " WHERE "
                  + database::detail::field_name<T>::value + " = :value");
        q.bindValue(":value", value);
        q.exec();
        QList<typename database::detail::result_type<T>::value> items;
        while(q.next())
            items.push_back(map<typename detail::result_type<T>::value>(q));
        return items;
    }

    template<typename T>
    void find_by_many_build_query(QString& query, const typename database::detail::field_type<T>::value& t) {
        query += QString(" AND ") + database::detail::field_name<T>::value + " = :" + database::detail::field_name<T>::value;
    }

    template<typename T, typename... Ts>
    void find_by_many_build_query(QString& query,
                                  const typename database::detail::field_type<T>::value& t,
                                  const typename database::detail::field_type<Ts>::value&... ts) {
        query += QString(" AND ") + database::detail::field_name<T>::value + " = :" + database::detail::field_name<T>::value;
        find_by_many_build_query<Ts...>(query, ts...);
    }

    template<typename T>
    void find_by_many_bind_values(QSqlQuery& q, const typename database::detail::field_type<T>::value& t) {
        q.bindValue(QString(":") + database::detail::field_name<T>::value, t);
    }

    template<typename T, typename... Ts>
    void find_by_many_bind_values(QSqlQuery& q,
                                  const typename database::detail::field_type<T>::value& t,
                                  const typename database::detail::field_type<Ts>::value&... ts) {
        q.bindValue(QString(":") + database::detail::field_name<T>::value, t);
        find_by_many_bind_values<Ts...>(q, ts...);
    }

    template<typename T, typename... Ts>
    QList<typename database::detail::result_type<T>::value>
    find_by_many(QSqlDatabase& db,
                 const typename database::detail::field_type<T>::value& value,
                 const typename database::detail::field_type<Ts>::value&... ts) {
        QSqlQuery q { db };
        QString query = QString("SELECT * FROM ") + database::detail::table_name<T>::value + " WHERE "
                + database::detail::field_name<T>::value + " = :" + database::detail::field_name<T>::value;
        find_by_many_build_query<Ts...>(query, ts...);
        q.prepare(query);
        q.bindValue(QString(":") + database::detail::field_name<T>::value, value);
        find_by_many_bind_values<Ts...>(q, ts...);
        q.exec();
        QList<typename database::detail::result_type<T>::value> items;
        while(q.next())
            items.push_back(map<typename detail::result_type<T>::value>(q));
        return items;
    }

    template<typename Field>
    void update_build_set(QString& query, const typename database::detail::field_type<Field>::value &field) {
        query += QString(", ") + database::detail::field_name<Field>::value + " = :"
                + database::detail::field_name<Field>::value;
    }

    template<typename Field, typename... Fields>
    void update_build_set(QString& query, const typename database::detail::field_type<Field>::value &field,
                          const typename database::detail::field_type<Fields>::value&... fields) {
        query += QString(", ") + database::detail::field_name<Field>::value + " = :"
                + database::detail::field_name<Field>::value;
        update_build_set<Fields...>(query, fields...);
    }

    template<typename Item, typename Field, typename... Fields>
    typename std::enable_if<
        !std::is_same<
            std::integral_constant<int, sizeof...(Fields)>,
            std::integral_constant<int, 0>
        >::value>::type update(QSqlDatabase& db, const Item& item,
                const typename database::detail::field_type<Field>::value& field,
                const typename database::detail::field_type<Fields>::value&... fields) {
        QSqlQuery q { db };
        QString query = QString("UPDATE ") + database::detail::name<Item>::value + " SET "
                + database::detail::field_name<Field>::value + " = :"
                + database::detail::field_name<Field>::value;
        update_build_set<Fields...>(query, fields...);
        query += " WHERE :id = id";
        q.prepare(query);
        q.bindValue(QString(":") + database::detail::field_name<Field>::value, field);
        q.bindValue(":id", item.id());
        // Looks wierd, but find_by_many_bind_values could just be called bind_values
        find_by_many_bind_values<Fields...>(q, fields...);
        q.exec();
    }

    template<typename Item, typename Field, typename... Fields>
    typename std::enable_if<
        std::is_same<
            std::integral_constant<int, sizeof...(Fields)>,
            std::integral_constant<int, 0>
        >::value>::type update(QSqlDatabase& db, const Item& item,
                               const typename database::detail::field_type<Field>::value& field,
                               const typename database::detail::field_type<Fields>::value&... fields) {
        QSqlQuery q { db };
        QString query = QString("UPDATE ") + database::detail::name<Item>::value + " SET "
                + database::detail::field_name<Field>::value + " = :"
                + database::detail::field_name<Field>::value;
        query += " WHERE :id = id";
        q.prepare(query);
        q.bindValue(QString(":") + database::detail::field_name<Field>::value, field);
        q.bindValue(":id", item.id());
        q.exec();
    }
}

/**
  Here be dragons
  */
#define EXPAND_TAGS(r, data, elem) \
    struct BOOST_PP_TUPLE_ELEM(0, elem) {};

#define EXPAND_GETTERS(r, data, elem) \
    template<> \
    struct get<database :: BOOST_PP_TUPLE_ELEM(1, data) :: BOOST_PP_TUPLE_ELEM(0, elem)> {  \
        static BOOST_PP_TUPLE_ELEM(1, elem) apply(const BOOST_PP_TUPLE_ELEM(0, data)& a) { \
            return a.property(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem))).value<BOOST_PP_TUPLE_ELEM(1, elem)>(); \
        } \
    };

#define EXPAND_SETTERS(r, data, elem) \
    template<> \
    struct set<database :: BOOST_PP_TUPLE_ELEM(1, data) :: BOOST_PP_TUPLE_ELEM(0, elem)> { \
        static BOOST_PP_TUPLE_ELEM(1, elem) apply(BOOST_PP_TUPLE_ELEM(0, data)& a, const BOOST_PP_TUPLE_ELEM(1, elem)& value) { \
            a.setProperty(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)), value); \
        } \
    };

#define EXPAND_FIELD_TYPES(r, data, elem) \
    template<> \
    struct field_type<database :: BOOST_PP_TUPLE_ELEM(1, data) :: BOOST_PP_TUPLE_ELEM(0, elem)> { \
        using value = BOOST_PP_TUPLE_ELEM(1, elem); \
    };

#define EXPAND_TABLES(r, data, elem) \
    template<> \
    struct table_name<database :: BOOST_PP_TUPLE_ELEM(1, data) :: BOOST_PP_TUPLE_ELEM(0, elem)> { \
        static constexpr const char* value = BOOST_PP_TUPLE_ELEM(2, data); \
    };

#define EXPAND_FIELD_NAMES(r, data, elem) \
    template<> \
    struct field_name<database :: BOOST_PP_TUPLE_ELEM(1, data) :: BOOST_PP_TUPLE_ELEM(0, elem)> { \
        static constexpr const char* value = BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)); \
    };

#define EXPAND_RESULT_TYPES(r, data, elem) \
    template<> \
    struct result_type<database :: BOOST_PP_TUPLE_ELEM(1, data) :: BOOST_PP_TUPLE_ELEM(0, elem)> { \
        using value = BOOST_PP_TUPLE_ELEM(0, data); \
    };

#define EXPAND_MAPPINGS(index, count, mappings) \
    boost::mpl::pair<database :: BOOST_PP_TUPLE_ELEM(0, mappings) :: BOOST_PP_TUPLE_ELEM(0, BOOST_PP_SEQ_ELEM(count, BOOST_PP_TUPLE_ELEM(1,mappings))), \
        BOOST_PP_TUPLE_ELEM(1, BOOST_PP_SEQ_ELEM(count, BOOST_PP_TUPLE_ELEM(1, mappings)))>


#define EQ_PRED(r, state) \
    BOOST_PP_NOT_EQUAL ( \
        BOOST_PP_TUPLE_ELEM(0, state), \
        BOOST_PP_TUPLE_ELEM(1, state) \
    ) \

#define EQ_OP(r, state) \
    ( \
        BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(0, state)), \
        BOOST_PP_TUPLE_ELEM(1, state), \
        BOOST_PP_TUPLE_ELEM(2, state), \
        BOOST_PP_TUPLE_ELEM(3, state) \
    ) \

#define LIST_TRANSFORM(d, data, elem) \
    data :: elem

#define EXPAND_QUERIES(r, state) \
    template<> \
    struct query< BOOST_PP_TUPLE_ELEM(0, BOOST_PP_TUPLE_ELEM(3, state)), \
                  queries :: BOOST_PP_TUPLE_ELEM(0, BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(0, state), BOOST_PP_TUPLE_ELEM(2, state))) \
                  > { \
        static constexpr const char* value = BOOST_PP_TUPLE_ELEM(1, BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(0, state), BOOST_PP_TUPLE_ELEM(2, state))); \
        using params = boost::mpl::vector< BOOST_PP_LIST_ENUM( \
            BOOST_PP_LIST_TRANSFORM( \
                LIST_TRANSFORM, \
                BOOST_PP_TUPLE_ELEM(1, BOOST_PP_TUPLE_ELEM(3, state)), \
                BOOST_PP_TUPLE_TO_LIST(BOOST_PP_TUPLE_ELEM(2, BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_ELEM(0, state), BOOST_PP_TUPLE_ELEM(2, state)))) \
            )\
        ) >; \
    };

#define CREATE_TABLE(class_name, ns, title, queries, mappings) \
    namespace database { \
        namespace ns { \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_TAGS, _, mappings) \
        } \
        namespace detail { \
            template<>	\
            struct name<class_name> { \
                static constexpr const char* value = title; \
            }; \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_GETTERS, (class_name, ns), mappings) \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_SETTERS, (class_name, ns), mappings) \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_TABLES, (class_name, ns, title), mappings) \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_TYPES, (class_name, ns), mappings) \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_FIELD_NAMES, (class_name, ns), mappings) \
            BOOST_PP_SEQ_FOR_EACH(EXPAND_RESULT_TYPES, (class_name, ns), mappings) \
            template<> \
            struct mapper<class_name> { \
                using value  = boost::mpl::map< \
                    BOOST_PP_ENUM(BOOST_PP_SEQ_SIZE(mappings), EXPAND_MAPPINGS, (ns, mappings)) \
                >; \
            }; \
            BOOST_PP_FOR((0, BOOST_PP_TUPLE_SIZE(queries), queries, (class_name, ns)), \
                EQ_PRED, EQ_OP, EXPAND_QUERIES) \
        } \
    }

CREATE_TABLE(
    Album, albums, "Albums",
    (
        (create, "CREATE TABLE IF NOT EXISTS Albums(id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, artist TEXT, image BLOB)"),
        (find, "SELECT * FROM Albums WHERE title = :title AND artist = :artist", (title, artist)),
        (find_all, "SELECT * FROM Albums"),
        (exists, "SELECT COUNT(id) FROM Albums WHERE id = :id", (id)),
        (insert, "INSERT INTO Albums(title, artist, image) VALUES(:title, :artist, :image)", (title, artist, art))
    )
    ,
    ((id, quint64))
    ((title, QString))
    ((artist, QString))
    ((art, QString))
)

CREATE_TABLE(
    Song, songs, "Songs",
    (
        (create, "CREATE TABLE IF NOT EXISTS Songs(id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, title TEXT, artist TEXT, album TEXT, art TEXT)"),
        (find, "SELECT * FROM Songs WHERE title = :title AND artist = :artist AND album = :album", (title, artist, album)),
        (find_all, "SELECT * FROM Songs"),
        (exists, "SELECT COUNT(id) FROM Songs WHERE id = :id", (id)),
        (insert, "INSERT INTO Songs(path, title, artist, album, art) VALUES(:path, :title, :artist, :album)", (path, title, artist, album))
    ),
    ((id, quint64))
    ((path, QString))
    ((title, QString))
    ((artist, QString))
    ((album, QString))
)

CREATE_TABLE(
    Artist, artists, "Artists",
    (
        (create, "CREATE TABLE IF NOT EXISTS Artists(id INTEGER PRIMARY KEY AUTOINCREMENT, artist TEXT"),
        (find, "SELECT * FROM Artists WHERE artist = :artist", (artist)),
        (find_all, "SELECT * FROM Artists"),
        (exists, "SELECT COUNT(id) FROM Artists WHERE id = :id", (id)),
        (insert, "INSERT INTO Artists(artist) VALUES(:artist)", (artist))
    ),
    ((id, quint64))
    ((artist, QString))
)

CREATE_TABLE(
    Setting, settings, "Settings",
    (
        (create, "CREATE TABLE IF NOT EXISTS Settings(id INTEGER PRIMARY KEY AUTOINCREMENT, setting TEXT, value TEXT)"),
        (find, "SELECT * FROM Settings WHERE setting = :setting AND value = :value", (setting, value)),
        (find_all, "SELECT * FROM Settings"),
        (exists, "SELECT COUNT(id) FROM Settings WHERE id = : id", (id)),
        (insert, "INSERT INTO Settings(setting, value) VALUES(:setting, :value)", (setting, value))
    ),
    ((id, quint64))
    ((setting, QString))
    ((value, QString))
)

#endif
