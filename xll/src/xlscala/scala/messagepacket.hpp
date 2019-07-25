#ifndef XLSCALA_HPP_MESSAGEPACKET
#define XLSCALA_HPP_MESSAGEPACKET

#include <xlscala/defines.hpp>
#include <map>
#include <xlscala/xl/xlsession.hpp>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <boost/lexical_cast.hpp>
#include <fstream>

#include <winsock2.h>



namespace xlscala {

    template <int MaxSize>
    class ScalarToPacket: public xlscala::static_visitor<void> {
    protected:
        inline void checkspace(Size size) const {
             REQUIRE((*_index) + size < MaxSize, "not enought space in MessagePack, max = " << MaxSize);
        }

	public:
		ScalarToPacket(char *data, int *index)
		: _data(data), _index(index) {
		}

		inline void operator()(const Integer &i) const {
            checkspace(5);
            _data[(*_index)++] = 'i';
		    int *p = (int*)&(_data[(*_index)]);
		    *p = i;
		    (*_index) += 4;
		}

		inline void operator()(const Real &r) const {
		    checkspace(9);
		    _data[(*_index)++] = 'd';
		    Real *p = (Real*)&(_data[(*_index)]);
		    *p = r;
		    (*_index) += 8;
		}

        inline void operator()(const bool &b) const {
            checkspace(2);
		    _data[(*_index)++] = 'o';
            if (b) _data[(*_index)++] = 1;
            else  _data[(*_index)++] = 0;
        }

		inline void operator()(const std::string& s) const {
		    int size = s.size();
		    checkspace(5 + size);
		    _data[(*_index)++] = 's';
            int *p = (int*)&(_data[(*_index)]);
		    *p = size;
		    (*_index) += 4;
            std::copy(s.begin(), s.end(), &(_data[(*_index)]));
		    (*_index) += size;
		}

		inline void operator()(const Null&) const {
             checkspace(1);
		    _data[(*_index)++] = 'n';
		}

		inline void operator()(const Error& e) const {
		    auto s = e.msg();
            int size = s.size();
		    checkspace(5 + size);
		    _data[(*_index)++] = 'e';
            int *p = (int*)&(_data[(*_index)]);
		    *p = size;
		    (*_index) += 4;
            std::copy(s.begin(), s.end(), &(_data[(*_index)]));
		    (*_index) += size;
		}

	private:
		mutable char *_data;
		mutable int *_index;
	};

    template <int MaxSize>
    class MessagePacket {
    protected:
        static Null _null;

        mutable char _data[MaxSize];
        mutable int _index;
        ScalarToPacket<MaxSize> _visitor;

        inline void checkspace(Size size) const {
             REQUIRE(_index + size < MaxSize, "not enought space in MessagePacket, max = " << MaxSize);
        }


    public:
        MessagePacket()
        : _index(0), _visitor(_data, &_index) {
        }

        ~MessagePacket() {
        }

        inline void clear() { _index = 0; }

        inline void fixIndex(Size size) const { _index = size; }

        inline char* data() const {
            return _data;
        }

        inline Size size() const { return _index; }

        inline Size maxsize() const { return MaxSize; }

        inline void insertError(const std::string& v) const { insert(Error(v)); }

        inline void insertChar(const char& c) const {
            checkspace(2);
            _data[_index++] = 'c';
            _data[_index++] = c;
        }

        inline void insert(const Scalar& scalar) const {
            xlscala::apply_visitor(_visitor, scalar);
        }

        inline void insert(const std::vector<Scalar>& scalars) const {
            int size = scalars.size();
		    checkspace(5);
		    _data[_index++] = '*';
            int *p = (int*)&(_data[_index]);
		    *p = size;
		    _index += 4;
            for (Size i=0;i<size;++i) {
                xlscala::apply_visitor(_visitor, scalars[i]);
            }
        }

        inline std::string getError() const {
            if (_data[_index] == 'e') {
                int *p = (int *)&(_data[_index + 1]);
                int size = *p;
                std::string ret(size, '\0');
                std::copy(&(_data[_index + 5]), &(_data[_index + 5 + size]), ret.begin());
                _index += (size + 5);
                return ret;
            } else {
                return "";
            }
        }

        inline Scalar getScalar() const {
            switch (_data[_index]) {
                case 'f': {
                    float *p = (float *)&(_data[_index + 1]);
                    float v = *p;
                    _index += 5;
                    return double(v);
                }
                case 'd': {
                    double *p = (double *)&(_data[_index + 1]);
                    double v = *p;
                    _index += 9;
                    return v;
                }
                case 'i': {
                    int *p = (int *)&(_data[_index + 1]);
                    int v = *p;
                    _index += 5;
                    return v;
                }
                case 't': {
                    short *p = (short *)&(_data[_index + 1]);
                    short v = *p;
                    _index += 3;
                    return Integer(v);
                }
                case 'l': {
                    long *p = (long *)&(_data[_index + 1]);
                    long v = *p;
                    _index += 9;
                    return Integer(v);;
                }
                case 'b': {
                    _index += 2;
                    return Integer(_data[_index-1]);
                }
                case 'c': {
                    _index += 2;
                    return Integer(_data[_index-1]);
                }
                case 'u': {
                    _index += 1;
                    return _null;
                }
                case 'n': {
                    _index += 1;
                    return _null;
                }
                case 'o': {
                    _index += 2;
                    if (_data[_index-1] == 0) {
                        return false;
                    } else {
                        return true;
                    }
                }
                case 's': {
                    int *p = (int *)&(_data[_index + 1]);
                    int size = *p;
                    std::string ret(size, '\0');
                    std::copy(&(_data[_index + 5]), &(_data[_index + 5 + size]), ret.begin());
                    _index += (size + 5);
                    return ret;
                }
                case 'e': {
                    int *p = (int *)&(_data[_index + 1]);
                    int size = *p;
                    std::string ret(size, '\0');
                    std::copy(&(_data[_index + 5]), &(_data[_index + 5 + size]), ret.begin());
                    _index += (size + 5);
                    FAIL(ret);
                }
                default: FAIL ("not a scalar type");
            }
        }

        inline std::vector<Scalar> getScalarVector() const {
            switch (_data[_index]) {
                case '*': {
                    int *p = (int *)&(_data[_index + 1]);
                    int size = *p;
                    _index += 5;
                    std::vector<Scalar> ret(size);
                    for (int i=0;i<size;++i) {
                        ret[i] = getScalar();
                    }
                    return ret;
                }
                default: FAIL ("not a scalar vector type");
            }
        }

    };
}



#endif



