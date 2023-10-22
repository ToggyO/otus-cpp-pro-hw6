MATRIX_TEMPLATE_ARGS
MATRIX_INDEXER_TEMPLATE_ARGS
MATRIX_INDEXER_TYPE::Indexer(MatrixType::KeyType &key, MatrixType::MatrixType &matrix)
    : m_key{key}, m_matrix{matrix} {}

MATRIX_TEMPLATE_ARGS
MATRIX_INDEXER_TEMPLATE_ARGS
auto MATRIX_INDEXER_TYPE::operator[](size_t index)
{
    m_key[DIM_SIZE - DIMENSION] = index;
    return Indexer<DIMENSION - 1>(m_key, m_matrix);
}

MATRIX_TEMPLATE_ARGS
MATRIX_INDEXER_TEMPLATE_ARGS_0D
MATRIX_INDEXER_TYPE_0D::Indexer(MatrixType::KeyType &key, MatrixType::MatrixType &matrix)
    : m_key{key}, m_matrix{matrix} {}

MATRIX_TEMPLATE_ARGS
MATRIX_INDEXER_TEMPLATE_ARGS_0D
MATRIX_INDEXER_TYPE_0D& MATRIX_INDEXER_TYPE_0D::operator=(const TValue &value)
{
    if (value == m_matrix.default_value)
    {
        m_matrix.m_storage.erase(m_key);
    }
    else
    {
        m_matrix.m_storage[m_key] = TValue(value);
    }
    return *this;
}

MATRIX_TEMPLATE_ARGS
MATRIX_INDEXER_TEMPLATE_ARGS_0D
MATRIX_INDEXER_TYPE_0D& MATRIX_INDEXER_TYPE_0D::operator=(TValue &&value)
{
    if (value == m_matrix.default_value)
    {
        m_matrix.m_storage.erase(m_key);
    }
    else
    {
        m_matrix.m_storage[m_key] = std::move(value);
    }
    return *this;
}