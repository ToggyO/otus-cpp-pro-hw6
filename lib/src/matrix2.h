#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "traits.h"
#include "utils.h"

// ***************************************************************************
//                     TEMPLATE TYPES REPLACEMENT DEFINITIONS
// ***************************************************************************

#define MATRIX_TEMPLATE_ARGS            template <typename TValue, std::size_t DIM_SIZE>
#define MATRIX_TYPE                     Matrix<TValue, DIM_SIZE>
#define MATRIX_INDEXER_TEMPLATE_ARGS    template <size_t DIMENSION, typename Dummy>
#define MATRIX_INDEXER_TEMPLATE_ARGS_0D template <typename Dummy>
#define MATRIX_INDEXER_TYPE             MATRIX_TYPE::Indexer<DIMENSION, Dummy>
#define MATRIX_INDEXER_TYPE_0D          MATRIX_TYPE::Indexer<0, Dummy>

// ***************************************************************************

/**
 * @brief N-dimensional matrix representation.
 *
 * @tparam TValue type of stored values.
 *
 * @tparam DIM_SIZE dimensions count.
 */
template <typename TValue, std::size_t DIM_SIZE>
class Matrix final
{
private:
    using KeyType = std::array<TValue, DIM_SIZE>;
    using MatrixType = Matrix<TValue, DIM_SIZE>;

    struct Hash
    {
        std::size_t operator()(const KeyType& indexes) const {
            std::size_t h = 0;
            for (auto e : indexes) {
                h ^= std::hash<TValue>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };

    using Storage = std::unordered_map<KeyType, TValue, Hash>;
    using Iterator = typename Storage::iterator;
    using ConstIterator = typename Storage::const_iterator;

public:
    /**
     * @brief Proxy object for Matrix subscript operator.
     *
     * @tparam DIMENSION dimension identifier, that proxy object represents.
     *
     * @tparam Dummy stub object type.
     */
    template <size_t DIMENSION, typename Dummy = void> // error: explicit specialization in non-namespace scope ‘class Matrix<TValue, DIM_SIZE, DEFAULT_VALUE>’
    class Indexer
    {
    public:
        /**
         * @brief Constructs object of type `Indexer`.
         *
         * @param KeyType& storage key.
         *
         * @brief MatrixType& reference to matrix instance.
         */
        Indexer(KeyType&, MatrixType&);

        /**
         * @brief Proxy object subscript operator.
         *
         * @return Proxy object with lowered dimension number.
         */
        [[nodiscard]]
        auto operator[](size_t);

    private:
        KeyType m_key;
        MatrixType &m_matrix;
    };

    /**
     * @brief Specialization for proxy object for Matrix subscript operator.
     *
     * @tparam Dummy stub object type.
     */
    template<typename Dummy> // error: explicit specialization in non-namespace scope ‘class Matrix<TValue, DIM_SIZE, DEFAULT_VALUE>’
    class Indexer<0, Dummy>
    {
    public:
        /**
         * @brief Constructs object of type `Indexer`.
         *
         * @param KeyType& storage key.
         *
         * @brief MatrixType& reference to matrix instance.
         */
        Indexer(KeyType&, MatrixType&);

        /**
         * @brief Proxy object assign operator for lvalue argument.
         *
         * @param TValue assignable value const reference.
         *
         * @return proxy object.
         */
        Indexer<0, Dummy>& operator=(const TValue &value);

        /**
         * @brief Proxy object assign operator for lvalue argument.
         *
         * @param TValue assignable value universal reference.
         *
         * @return proxy object.
         */
        Indexer<0, Dummy>& operator=(TValue &&value);

        /**
         * @brief implicit conversion operator to matrix stored value type.
         *
         * @return instance of matrix stored value type
         */
        operator TValue() const { return m_matrix.get_value(m_key); }

    private:
        KeyType m_key;
        MatrixType& m_matrix;
    };

    /**
     * @brief Constructs new instance of Matrix.
     *
     * @param def_val default value of matrix stored value type.
     */
    explicit Matrix(const TValue& def_val) : default_value{std::move(TValue(def_val))}
    {}

    /**
     * @brief Copy constructor.
     *
     * @param other Copyable instance.
     */
    Matrix(const MatrixType& other)
    {
        m_storage = other.m_storage;
    }

    /**
     * @brief Move constructor.
     *
     * @param other Movable instance.
     */
    Matrix(MatrixType&& other) noexcept
    {
        m_storage = std::move(other.m_storage);
    }

    ~Matrix() = default;

    /**
     * @brief Return count of stored values.
     *
     * @return Count of stored values.
     */
    [[nodiscard]]
    size_t size() const { return m_storage.size(); }

    /**
     * @brief Copy assignment operator.
     *
     * @param other copiable instance.
     *
     * @return Matrix instance.
     */
    MatrixType& operator=(const MatrixType&);

    /**
     * @brief Move assignment operator.
     *
     * @param other copiable instance.
     *
     * @return Matrix instance.
     */
    MatrixType& operator=(MatrixType&&) noexcept;

    /**
     * @brief Subscript operator.
     *
     * @param index index of integral type.
     *
     * @return Proxy object.
     */
    [[nodiscard]]
    auto operator[](size_t);

    /**
     * @brief Const subscript operator.
     *
     * @param index index of integral type.
     *
     * @return Proxy object.
     */
    [[nodiscard]]
    auto operator[](size_t) const;

    /**
     * @brief Returns iterator of start position.
     */
    [[nodiscard]]
    Iterator begin() { return m_storage.begin(); }

    /**
     * @brief Returns iterator of end position.
     */
    [[nodiscard]]
    Iterator end() { return m_storage.end(); }

    /**
     * @brief Constant version. Returns const iterator of start position.
     */
    [[nodiscard]]
    Iterator begin() const { return m_storage.cbegin(); }

    /**
     * @brief Constant version. Returns  iterator of end position.
     */
    [[nodiscard]]
    Iterator end() const { return m_storage.cend(); }

    /**
     * @brief Returns const iterator of start position.
     */
    [[nodiscard]]
    ConstIterator cbegin() const { return m_storage.cbegin(); }

    /**
     * @brief Returns const iterator of end position.
     */
    [[nodiscard]]
    ConstIterator cend() const { return m_storage.cend(); }

private:
    TValue& get_value(const KeyType& key)
    {
        auto search = m_storage.find(key);
        if (search == m_storage.end())
        {
            return default_value;
        }

        return search->second;
    }

    TValue default_value;
    Storage m_storage;
};

#include "matrix_proxy.inl"
#include "matrix_operators.inl"
