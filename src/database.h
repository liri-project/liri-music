#ifndef LIRI_MUSIC_DATABASE_H
#define LIRI_MUSIC_DATABASE_H

/*
 * API That should be supported:
 * Album = find<album::id>(id);
 *
 * Album = find<album:id, album::artist>(id, artist);
 *
 * insert<
 * 	set<album::id, album::artist, album::title>
 * >(album);
 *
 * insert(db, album); // Should also work
 *
 * create<Album>();
 *
 * update<
 * 	set<album::artist, album::title>,
 * 	where<album::id>>(album);
 *
 * delete<where<album::id>>(id);
 */
namespace database
{
    template<typename T>
    struct Table {};

    template<typename... Ts>
    struct Columns {};

    template<typename T>
    struct ColumnTraits {};

    template<typename... Ts>
    struct set {};

    template<typename... Ts>
    struct where {};

    namespace detail {
        //--------------------------------------
        // The Next two structs are just helpers
        // that get the first type from a list
        // of types (eg. int from a list of
        // int, std::string, long long
        template<typename T, typename... Ts>
        struct FirstOfImpl {
            using value = T;
        };

        template<typename... Ts>
        struct FirstOf : FirstOfImpl<Ts...> {
        };
        //--------------------------------------

        //--------------------------------------
        // Forward declare a type for mapping
        // queries into QObjects
        template<typename Item, typename T, typename... Ts>
        struct Mapper;

        // Base case to terminate template
        // instatiations
        template<typename Item, typename T>
        struct Mapper<Item, T> {
            static void apply(QSqlQuery& q, Item& item) {
                item.setProperty(ColumnTraits<T>::name, q.value(ColumnTraits<T>::name).template value<typename ColumnTraits<T>::type>());
            }
        };

        // Recursive case expands elements into
        // code that does the mapping
        template<typename Item, typename T, typename... Ts>
        struct Mapper {
            static void apply(QSqlQuery& q, Item& item) {
                item.setProperty(ColumnTraits<T>::name, q.value(ColumnTraits<T>::name).template value<typename ColumnTraits<T>::type>());
                Mapper<Item, Ts...>::apply(q, item);
            }
        };

        // Helper to get types out of the
        // Columns<...> type
        template<typename Item, typename... Ts>
        struct Mapper<Item, Columns<Ts...>> {
            static void apply(QSqlQuery& query, Item& item) {
                Mapper<Item, Ts...>::apply(query, item);
            }
        };
        //--------------------------------------

        //--------------------------------------
        // These structs are used to build a
        // comparison clause for a select query
        template<typename T, typename... Ts>
        struct ComparisonClauseBuilder;

        template<typename T>
        struct ComparisonClauseBuilder<T> {
            static void apply(QString& query) {
                query += ColumnTraits<T>::name;
                query += " = :";
                query += ColumnTraits<T>::name;
            }
        };

        template<typename T, typename... Ts>
        struct ComparisonClauseBuilder {
            static void apply(QString& query) {
                query += ColumnTraits<T>::name;
                query += " = :";
                query += ColumnTraits<T>::name;
                query += " AND ";
                ComparisonClauseBuilder<Ts...>::apply(query);
            }
        };
        //--------------------------------------

        //--------------------------------------
        // These structs are used to bind a list
        // of values to a query
        template<typename T, typename... Ts>
        struct ValueBinder;

        template<typename T>
        struct ValueBinder<T> {
            static void apply(QSqlQuery& query,
                              const typename ColumnTraits<T>::type& value) {
                query.bindValue(QString(":") + ColumnTraits<T>::name,
                                value);
            }
        };

        template<typename T, typename... Ts>
        struct ValueBinder {
            static void apply(QSqlQuery& query,
                              const typename ColumnTraits<T>::type& value,
                              const typename ColumnTraits<Ts>::type&... values) {
                query.bindValue(QString(":") + ColumnTraits<T>::name,
                                value);
                ValueBinder<Ts...>::apply(query, values...);
            }
        };
        //--------------------------------------

        //--------------------------------------
        // These struct are used to build a
        // table query based on the column names
        template<typename T, typename... Ts>
        struct TableQueryBuilder;

        template<typename T>
        struct TableQueryBuilder<T> {
           static void apply(QString& query) {
               query += ColumnTraits<T>::name;
               query += " ";
               query += ColumnTraits<T>::dbType;
           }
        };

        template<typename T, typename... Ts>
        struct TableQueryBuilder {
            static void apply(QString& query) {
                query += ColumnTraits<T>::name;
                query += " ";
                query += ColumnTraits<T>::dbType;
                query += ",";
                TableQueryBuilder<Ts...>::apply(query);
            }
        };

        template<typename... Ts>
        struct TableQueryBuilder<Columns<Ts...>> {
            static void apply(QString& query) {
                TableQueryBuilder<Ts...>::apply(query);
            }
        };
        //--------------------------------------
        // Builds a list of columns for an
        // insert statement
        template<typename T, typename... Ts>
        struct InsertQueryBuilder;

        template<typename T>
        struct InsertQueryBuilder<T> {
            static void apply(QString& query) {
                if(QString(ColumnTraits<T>::name)
                        != "id") {
                    query += ColumnTraits<T>::name;
                } else {
                    query.chop(1);
                }
            }
        };

        template<typename T, typename... Ts>
        struct InsertQueryBuilder {
            static void apply(QString& query) {
                if(QString(ColumnTraits<T>::name)
                        != "id") {
                    query += ColumnTraits<T>::name;
                    query += ",";
                }
                InsertQueryBuilder<Ts...>::apply(query);
            }
        };

        template<typename... Ts>
        struct InsertQueryBuilder<Columns<Ts...>> {
            static void apply(QString& query) {
                InsertQueryBuilder<Ts...>::apply(query);
            }
        };
        //--------------------------------------

        //--------------------------------------
        // Builds a list of strings to use as
        // identifiers for QSqlQuery.bindValue
        template<typename T, typename... Ts>
        struct InsertValueBuilder;

        template<typename T>
        struct InsertValueBuilder<T> {
            static void apply(QString& query) {
                if(QString(ColumnTraits<T>::name)
                        != "id") {
                    query += ":";
                    query += ColumnTraits<T>::name;
                } else {
                    query.chop(1);
                }
            }
        };

        template<typename T, typename... Ts>
        struct InsertValueBuilder {
            static void apply(QString& query) {
                if(QString(ColumnTraits<T>::name)
                        != "id") {
                    query += ":";
                    query += ColumnTraits<T>::name;
                    query += ",";
                }
                InsertValueBuilder<Ts...>::apply(query);
            }
        };

        template<typename... Ts>
        struct InsertValueBuilder<Columns<Ts...>> {
            static void apply(QString& query) {
                InsertValueBuilder<Ts...>::apply(query);
            }
        };
        //--------------------------------------

        //--------------------------------------
        // Bind values to an insert query
        // Can't just use ValueBinder because we
        // need to skip the id field
        template<typename T, typename... Ts>
        struct InsertBinder;

        template<typename T>
        struct InsertBinder<T> {
            static void apply(QSqlQuery& query,
                              const typename ColumnTraits<T>::belongs_to& v) {
                if(QString(ColumnTraits<T>::name) != "id") {
                    query.bindValue(QString(":") + ColumnTraits<T>::name,
                                    v.property(ColumnTraits<T>::name).template value<typename ColumnTraits<T>::type>());
                }
            }
        };

        template<typename T, typename... Ts>
        struct InsertBinder {
            static void apply(QSqlQuery& query, const typename ColumnTraits<T>::belongs_to& v) {
                if(QString(ColumnTraits<T>::name) != "id") {
                    query.bindValue(QString(":") + ColumnTraits<T>::name,
                                    v.property(ColumnTraits<T>::name).template value<typename ColumnTraits<T>::type>());
                }
                InsertBinder<Ts...>::apply(query, v);
            }
        };

        template<typename... Ts>
        struct InsertBinder<Columns<Ts...>> {
            static void apply(QSqlQuery& query,
                              const typename ColumnTraits<typename FirstOf<Ts...>::value>::belongs_to& value) {
                InsertBinder<Ts...>::apply(query, value);
            }
        };

        //--------------------------------------

        //--------------------------------------
        // Function frontend
        template<typename T>
        void map(QSqlQuery& q, T& item) {
            Mapper<T, typename Table<T>::columns>::apply(q, item);
        }
        //--------------------------------------
    }

    //--------------------------------------
    // Find a set of items in the database.
    // Takes a list of tags that ColumnTraits
    // is specialized for as parameters. The
    // parameters specified what columns will
    // be compared in the query. For example:
    // find<artist::title>("..") will expand to
    // SELECT * FROM Artists where title =
    // "..", which then gets mapped
    // back to a full Album object, which
    // gets stuff in QList and returned to
    // the caller.
    template<typename... Ts>
    auto find(QSqlDatabase& db, const typename ColumnTraits<Ts>::type&... values)
        -> typename std::enable_if<
            !std::is_destructible<typename detail::FirstOf<Ts...>::value>::value
            ,QList<typename ColumnTraits<typename detail::FirstOf<Ts...>::value>::belongs_to>
           >::type {
        QList<typename ColumnTraits<typename detail::FirstOf<Ts...>::value>::belongs_to> items;
        QSqlQuery query { db };
        QString queryString = "SELECT * FROM ";
        queryString += Table<typename ColumnTraits<typename detail::FirstOf<Ts...>::value>::belongs_to>::name;
        queryString += " WHERE ";
        detail::ComparisonClauseBuilder<Ts...>::apply(queryString);
        query.prepare(queryString);
        detail::ValueBinder<Ts...>::apply(query, values...);
        query.exec();
        while(query.next()) {
            typename ColumnTraits<typename detail::FirstOf<Ts...>::value>::belongs_to item;
            detail::map(query, item);
            items.push_back(item);
        }
        return items;
    }

    template<typename T>
    auto find(QSqlDatabase& db)
        -> typename std::enable_if<
            std::is_destructible<T>::value,
            QList<T>>::type
    {
        QList<T> items;
        QSqlQuery query { db };
        QString queryString = "SELECT * FROM ";
        queryString += Table<T>::name;
        query.prepare(queryString);
        query.exec();
        while(query.next()) {
            T item;
            detail::map(query, item);
            items.push_back(item);
        }
        return items;
    }

    template<typename T>
    void insert(QSqlDatabase& db, const T& item) {
        QSqlQuery query { db };
        QString queryString = "INSERT INTO ";
        queryString += Table<T>::name;
        queryString += "(";
        detail::InsertQueryBuilder<typename Table<T>::columns>::apply(queryString);
        queryString += ") VALUES (";
        detail::InsertValueBuilder<typename Table<T>::columns>::apply(queryString);
        queryString += ")";
        query.prepare(queryString);
        detail::InsertBinder<typename Table<T>::columns>::apply(query, item);
        query.exec();
    }

    //--------------------------------------
    // Create a table in the database for a
    // given type T
    template<typename T>
    auto create(QSqlDatabase& db)
        -> void {
        QString queryString = "CREATE TABLE IF NOT EXISTS ";
        queryString += Table<T>::name;
        queryString += "(";
        detail::TableQueryBuilder<typename Table<T>::columns>::apply(queryString);
        queryString += ")";
        QSqlQuery { queryString, db };
    }
}

#endif
