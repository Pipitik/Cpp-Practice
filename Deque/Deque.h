#ifndef DEQUE_H
#define DEQUE_H

#include <algorithm>
#include <exception>
#include <iterator>
#include <type_traits>

template <typename T>
class Deque
{
private:
	template <bool Is_const>
	class base_iterator;

	static constexpr int _SIZE_BUCKET = 1 << 10;
	static constexpr int _MINIMUM_MAP_SIZE = 4;

public:
	using value_type = T;
	using size_type = std::size_t;
	using reference = T&;
	using difference_type = std::ptrdiff_t;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = base_iterator<false>;
	using const_iterator = base_iterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:

	// Index of a map bucket
	class Index_bucket
	{
	public:
		using size_type = typename Deque::size_type;
		using difference_type = typename Deque::difference_type;

		Index_bucket() noexcept : _bucket(0), _offset(0) {}

		Index_bucket(size_type buck, size_type off) noexcept
			: _bucket(buck)
			, _offset(off) {}

		Index_bucket(const Index_bucket&) noexcept = default;

		Index_bucket& operator=(const Index_bucket&) noexcept = default;

		// Shift the offset in the bucket
		Index_bucket& operator++() noexcept
		{
			++_offset;
			_bucket += _offset / _SIZE_BUCKET;
			_offset %= _SIZE_BUCKET;
			return *this;
		}

		// Shift the offset in the bucket
		Index_bucket operator++(int) noexcept
		{
			Index_bucket ret(*this);
			++(*this);
			return ret;
		}

		// Shift the offset in the bucket
		Index_bucket& operator--() noexcept
		{
			if (_bucket != 0 or _offset != 0) {
				if (_offset == 0) {
					--_bucket;
				}
				_offset = (_offset - 1) % _SIZE_BUCKET;
			}
			return *this;
		}

		// Shift the offset in the bucket
		Index_bucket operator--(int) noexcept
		{
			Index_bucket ret(*this);
			--(*this);
			return ret;
		}

		[[nodiscard]] size_type bucket() const noexcept
		{
			return _bucket;
		}

		[[nodiscard]] size_type offset() const noexcept
		{
			return _offset;
		}

		void set_bucket(difference_type number) noexcept
		{
			_bucket = number;
		}

		void swap(Index_bucket& other) noexcept
		{
			std::swap(_bucket, other._bucket);
			std::swap(_offset, other._offset);
		}

	private:
		size_type _bucket; 	// Bucket of the map
		size_type _offset; 	// Offset from the begging of the bucket
	};

	template <bool Is_const>
	class base_iterator
	{
		friend Deque;
	public:
		using iterator_category = std::random_access_iterator_tag;

		using value_type = typename Deque::value_type;
		using size_type = typename Deque::size_type;
		using difference_type = typename Deque::difference_type;
		using pointer_type = std::conditional_t<Is_const, const value_type*, value_type*>;
		using reference_type = std::conditional_t<Is_const, const value_type&, value_type&>;

		base_iterator() noexcept : _map(nullptr), _offset(0) {}
		base_iterator(const base_iterator&) noexcept = default;
		base_iterator& operator=(const base_iterator&) noexcept = default;

		reference_type operator*() const noexcept
		{
			size_type bucket = _offset / _SIZE_BUCKET;
			size_type off = _offset % _SIZE_BUCKET;
			return _map[bucket][off];
		}

		pointer_type operator->() const noexcept
		{
			return &(**this);
		}

		[[nodiscard]] reference_type operator[](difference_type offset) const noexcept
		{
			return *(*this + offset);
		}

		base_iterator& operator++() noexcept
		{
			++_offset;
			return *this;
		}

		[[nodiscard]] base_iterator operator++(int) noexcept
		{
			base_iterator ret(*this);
			++(*this);
			return ret;
		}

		base_iterator& operator--() noexcept
		{
			--_offset;
			return *this;
		}

		[[nodiscard]] base_iterator operator--(int) noexcept
		{
			base_iterator ret(*this);
			--(*this);
			return ret;
		}

		base_iterator& operator+=(difference_type offset) noexcept
		{
			_offset += offset;
			return *this;
		}

		base_iterator& operator-=(difference_type offset) noexcept
		{
			_offset -= offset;
			return *this;
		}

		[[nodiscard]] base_iterator operator+(difference_type offset) const noexcept
		{
			base_iterator ret(*this);
			ret += offset;
			return ret;
		}

		[[nodiscard]] friend base_iterator
		operator+(difference_type offset, base_iterator it) noexcept
		{
			return it += offset;
		}

		[[nodiscard]] base_iterator operator-(difference_type offset) const noexcept
		{
			base_iterator ret(*this);
			ret -= offset;
			return ret;
		}

		[[nodiscard]] friend base_iterator
		operator-(difference_type offset, base_iterator it) noexcept
		{
			return it -= offset;
		}

		[[nodiscard]] difference_type
		operator-(const base_iterator& other) const noexcept
		{
			return static_cast<difference_type>(_offset - other._offset);
		}

		[[nodiscard]] bool operator==(const base_iterator& other) const noexcept
		{
			return _offset == other._offset;
		}

		[[nodiscard]] std::strong_ordering
		operator<=>(const base_iterator& other) const noexcept
		{
			return _offset <=> other._offset;
		}

		operator base_iterator<true>() const noexcept
		{
			return { _map, _offset };
		}

	private:
		value_type** _map;
		size_type _offset;

		base_iterator(value_type** map, size_type offset) noexcept
			: _map(map)
			, _offset(offset) {}
	};

public:
	Deque() noexcept
		: _size_map(0)
		, _map(nullptr)
		, _size(0)
		, _first()
		, _last() {}

	explicit Deque(size_type count) : Deque()
	{
		resize(count);
	}

	Deque(size_type count, const T& value) : Deque()
	{
		std::fill_n(std::back_inserter(*this), count, value);
	}

	Deque(const Deque& other) : Deque()
	{
		std::copy(other.begin(), other.end(), std::back_inserter(*this));
	}

	template
	<
		typename InIt,
		std::enable_if_t<
			std::is_base_of_v<
			std::input_iterator_tag,
			typename std::iterator_traits<InIt>::iterator_category>, int> = 0
	>
	Deque(InIt begin, InIt end) : Deque()
	{
		std::copy(begin, end, std::back_inserter(*this));
	}

	Deque(std::initializer_list<T> init) : Deque()
	{
		std::copy(init.begin(), init.end(), std::back_inserter(*this));
	}

	~Deque()
	{
		_tidy();
		delete[] _map;
	}

	Deque& operator=(Deque other)
	{
		swap(other);
		return *this;
	}

	void swap(Deque& other) noexcept
	{
		std::swap(_size_map, other._size_map);
		std::swap(_map, other._map);
		std::swap(_size, other._size);
		_first.swap(other._first);
		_last.swap(other._last);
	}

	void push_back(const T& value)
	{
		_push_back_internal(value);
	}

	void pop_back() noexcept
	{
		(_map[_last.bucket()][_last.offset()]).~T();
		--_last;
		--_size;
	}

	void push_front(const T& value)
	{
		_push_front_internal(value);
	}

	void pop_front() noexcept
	{
		(_map[_first.bucket()][_first.offset()]).~T();
		++_first;
		--_size;
	}

	void clear() noexcept
	{
		_tidy();
	}

	void resize(size_type count)
	{
		while (_size < count) {
			_push_back_internal();
		}

		while (_size > count) {
			pop_back();
		}
	}

	void resize(size_type count, const T& value)
	{
		while (_size < count) {
			_push_back_internal(value);
		}

		while (_size > count) {
			pop_back();
		}
	}

	iterator insert(const_iterator where, const T& value)
	{
		size_type off = static_cast<size_type>(begin() - where);

		if (off < _size / 2) {
			_push_front_internal(value);
			std::rotate(begin(), std::next(begin()), begin() + static_cast<difference_type>(1 + off));
		}
		else {
			_push_back_internal(value);
			std::rotate(begin() + static_cast<difference_type>(1 + off), std::prev(end()), end());
		}

		return begin() + static_cast<difference_type>(off);
	}

	iterator erase(const_iterator where)
	{
		return erase(where, std::next(where));
	}

	iterator erase(const_iterator first_arg, const_iterator last_arg)
	{
		iterator first = _make_iterator(first_arg);
		iterator last = _make_iterator(last_arg);
		size_type count = static_cast<size_type>(last - first);

		if (count == 0) {
			return last;
		}

		size_type off = static_cast<size_type>(begin() - first);
		if (off < static_cast<size_type>(end() - last)) {
			std::move_backward(begin(), first, last);
			for (; count > 0; --count) {
				pop_front();
			}
		}
		else {
			std::move(last, end(), first);
			for (; count > 0; --count) {
				pop_back();
			}
		}

		return begin() + static_cast<difference_type>(off);
	}

	[[nodiscard]] iterator begin() noexcept
	{
		return iterator(_map, _first.bucket() * _SIZE_BUCKET + _first.offset());
	}

	[[nodiscard]] iterator end() noexcept
	{
		return iterator(_map, _last.bucket() * _SIZE_BUCKET + _last.offset() + 1);
	}

	[[nodiscard]] const_iterator begin() const noexcept
	{
		return const_iterator(_map, _first.bucket() * _SIZE_BUCKET + _first.offset());
	}

	[[nodiscard]] const_iterator end() const noexcept
	{
		return const_iterator(_map, _last.bucket() * _SIZE_BUCKET + _last.offset() + 1);
	}

	[[nodiscard]] const_iterator cbegin() const noexcept
	{
		return begin();
	}

	[[nodiscard]] const_iterator cend() const noexcept
	{
		return end();
	}

	[[nodiscard]] reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(begin());
	}

	[[nodiscard]] reverse_iterator rend() noexcept
	{
		return reverse_iterator(end());
	}

	[[nodiscard]] const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	[[nodiscard]] const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(end());
	}

	[[nodiscard]] const_reverse_iterator crbegin() const noexcept
	{
		return rbegin();
	}

	[[nodiscard]] const_reverse_iterator crend() const noexcept
	{
		return rend();
	}

	[[nodiscard]] reference operator[](size_type index) noexcept
	{
		return *(begin() + static_cast<difference_type>(index));
	}

	[[nodiscard]] const_reference operator[](size_type index) const noexcept
	{
		return *(begin() + static_cast<difference_type>(index));
	}

	[[nodiscard]] reference at(size_type index)
	{
		if (_size <= index) {
			throw std::out_of_range("Deque out of range");
		}

		return *(begin() + static_cast<difference_type>(index));
	}

	[[nodiscard]] const_reference at(size_type index) const
	{
		if (_size <= index) {
			throw std::out_of_range("Deque out of range");
		}

		return *(begin() + static_cast<difference_type>(index));
	}

	[[nodiscard]] reference front() noexcept
	{
		return *begin();
	}

	[[nodiscard]] const_reference front() const noexcept
	{
		return *begin();
	}

	[[nodiscard]] reference back() noexcept
	{
		return *std::prev(end());
	}

	[[nodiscard]] const_reference back() const noexcept
	{
		return *std::prev(end());
	}

	[[nodiscard]] size_type size() const noexcept
	{
		return _size;
	}

	[[nodiscard]] bool empty() const noexcept
	{
		return _size == 0;
	}

private:
	size_type _size_map; 	// Size of array of pointers to blocks;
	value_type** _map; 		// Pointer to array of pointers to blocks
	size_type _size; 		// Number of elements
	Index_bucket _first; 	// The index of first element
	Index_bucket _last; 	// The index of past-the-end element

	template <typename... Targs>
	void _push_back_internal(const Targs&... args)
	{
		static_assert(sizeof...(args) <= 1, "_push_back_internal: too many arguments");

		if ((_last.bucket() == _size_map - 1 and
			_last.offset() == _SIZE_BUCKET - 1) or empty()) {
			_reallocate_map(_size_map == 0 ? _MINIMUM_MAP_SIZE : _size_map * 2);
		}

		Index_bucket new_last = _last;

		if (!empty()) {
			++new_last;
		}

		if (_map[new_last.bucket()] == nullptr) {
			_map[new_last.bucket()] =
				reinterpret_cast<T*>(new std::byte[_SIZE_BUCKET * sizeof(T)]);

			try {
				new(_map[new_last.bucket()] + new_last.offset()) T(args...);
			}
			catch (...) {
				delete[] reinterpret_cast<std::byte*>(_map[new_last.bucket()]);
				_map[new_last.bucket()] = nullptr;
				throw;
			}
		}
		else {
			new(_map[new_last.bucket()] + new_last.offset()) T(args...);
		}

		_last = new_last;
		++_size;
	}

	template <typename... Targs>
	void _push_front_internal(const Targs&... args)
	{
		static_assert(sizeof...(args) <= 1, "_push_front_internal: too many arguments");

		if (_first.bucket() == 0 and _first.offset() == 0) {
			_reallocate_map(_size_map == 0 ? _MINIMUM_MAP_SIZE : _size_map * 2);
		}

		Index_bucket new_first = _first;

		if (!empty()) {
			--new_first;
		}

		if (_map[new_first.bucket()] == nullptr) {
			_map[new_first.bucket()] = reinterpret_cast<T*>(new std::byte[_SIZE_BUCKET * sizeof(T)]);

			try {
				new(_map[new_first.bucket()] + new_first.offset()) T(args...);
			}
			catch (...) {
				delete[] reinterpret_cast<std::byte*>(_map[new_first.bucket()]);
				_map[new_first.bucket()] = nullptr;
				throw;
			}
		}
		else {
			new(_map[new_first.bucket()] + new_first.offset()) T(args...);
		}

		_first = new_first;
		++_size;
	}

	void _reallocate_map(size_type new_size_map)
	{
		T** new_map = new T*[new_size_map]();

		size_type middle_map = _size_map / 2;

		if (_map != nullptr) {
			for (size_type index = _first.bucket(); index <= _last.bucket(); ++index) {
				new_map[middle_map - index] = _map[index]; // Try to keep pointers in the middle of the map
			}
			delete[] _map;
		}

		_map = new_map;
		_first.set_bucket(static_cast<difference_type>(middle_map) - _first.bucket());
		_last.set_bucket(static_cast<difference_type>(middle_map) - _last.bucket());
		_size_map = new_size_map;
	}

	void _tidy() noexcept
	{
		size_type first_bucket = _first.bucket();
		size_type last_bucket = _last.bucket();

		while (!empty()) {
			pop_back();
		}

		if (_map != nullptr) {
			for (size_type index = first_bucket; index <= last_bucket; ++index) {
				delete[] reinterpret_cast<std::byte*>(_map[index]);
				_map[index] = nullptr;
			}
		}
	}

	static iterator _make_iterator(const_iterator where) noexcept
	{
		return iterator(where._map, where._offset);
	}
};

template <typename T>
void swap(Deque<T>& lhs, Deque<T>& rhs) noexcept
{
	lhs.swap(rhs);
}

#endif
