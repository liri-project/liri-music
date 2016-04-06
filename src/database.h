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
        QSqlQuery q;
        q.prepare(Table<T>::insert::value);
        detail::query_binder<T> binder { q, item };
        boost::mpl::for_each<typename Table<T>::insert::params>(binder);
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
#endif
