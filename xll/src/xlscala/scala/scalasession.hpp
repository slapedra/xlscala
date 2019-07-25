#ifndef XLSCALA_HPP_PYSESSION
#define XLSCALA_HPP_PYSESSION

#include <xlscala/defines.hpp>
#include <map>
#include <xlscala/xl/xlsession.hpp>
#include <list>
#include <xlscala/scala/socketclient.hpp>
#include <xlscala/scala/messagepacket.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>


#define     XLSCALA_PORT        12345
#define     XLSCALA_HOST        "localhost"
#define     XLSCALA_MAXLINES    10000
#define     XLSCALA_BUFSIZE     4 * 1024 * 1024


namespace xlscala {

	class ScalaSession : public Singleton<ScalaSession> {
    friend class Singleton<ScalaSession>;
	public:

        ScalaSession()
        : _cout(XLSCALA_MAXLINES) {
        }

        ~ScalaSession() {
        };

        inline std::string host() const { return _socket.host(); }
        inline int port() const { return _socket.port(); }

        void init() {

        }

        void initConnection(const std::string& host, int port) {
            _socket.open(port, host);
        }



        inline bool connected() const { return _socket.connected(); }

        inline const boost::circular_buffer<std::string>& lines() const { return _cout; }

        inline void cout(const std::string& mmsg) const {
            std::string msg = mmsg;
            boost::replace_all(msg, "\t", "  ");
            std::vector<std::string> sv;
            boost::split(sv, msg, boost::is_any_of("\n") );
            for (Size i=0;i<sv.size();++i) {
                _cout.push_back(sv[i]);
            }
             _cout.push_back("");
        }

        inline Scalar set(const Scalar& scalar,
                          const std::shared_ptr< Range<Scalar> >& aargs,
                          const std::shared_ptr< Range<Scalar> >& vvalues,
                          const std::shared_ptr< Range<Scalar> >& iindex) const {
            std::vector<Scalar> args;
            std::vector<Scalar> bindings;
            std::vector<Scalar> values;
            std::vector<Scalar> indexes;
            if (iindex->size() > 0) {
                REQUIRE(vvalues->size() == iindex->size(), "index and values must have the same size");
                values.resize(vvalues->size());
                indexes.resize(vvalues->size());
                for(Size i=0;i<vvalues->size();++i) {
                    values[i] = vvalues->data[i];
                    indexes[i] = iindex->data[i];
                }
            } else {
                values.resize(vvalues->size());
                indexes.resize(vvalues->size());
                for(Size i=0;i<vvalues->size();++i) {
                    values[i] = vvalues->data[i];
                    indexes[i] = Scalar(_null);
                }
            }
            if (values.size() > 0) {
                args.resize(aargs->size() + 3);
                bindings.resize(aargs->size() + 3);
                for (Size i=0;i<aargs->size();++i) {
                    args[i] = aargs->data[i];
                    bindings[i] = Scalar(_null);
                }
                args[aargs->size() + 0] = Scalar(Integer(values.size()));
                args[aargs->size() + 1] = Scalar(Integer(vvalues->rows));
                args[aargs->size() + 2] = Scalar(Integer(vvalues->columns));
                bindings[aargs->size() + 0] = Scalar(std::string("size"));
                bindings[aargs->size() + 1] = Scalar(std::string("rows"));
                bindings[aargs->size() + 2] = Scalar(std::string("cols"));
            } else {
                args.resize(aargs->size());
                bindings.resize(aargs->size());
                for (Size i=0;i<aargs->size();++i) {
                    args[i] = aargs->data[i];
                    bindings[i] = Scalar(_null);
                }
            }
            auto pos = XLSession::instance().currentPosition();
            _msg.clear();
            _rmsg.clear();
            _msg.insertChar(char('s'));
            _msg.insert(Integer(pos->id()));
            _msg.insert(Integer(pos->row()));
            _msg.insert(Integer(pos->col()));
            _msg.insert(false);
            _msg.insert(scalar);
            _msg.insert(args);
            _msg.insert(bindings);
            _msg.insert(values);
            _msg.insert(indexes);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                ScalaSession::instance().cout(error);
                return Scalar(_null);
            } else {
                //return Scalar(1.0 * _rmsg.datasize());
                return _rmsg.getScalar();
            }
        }

        inline std::shared_ptr< Range<Scalar> > exec(const std::shared_ptr< Range<Scalar> >& scalars,
                                  const std::shared_ptr< Range<Scalar> >& aargs,
                                  bool transpose) const {
            std::vector<Scalar> args;
            std::vector<Scalar> bindings;
            args.resize(aargs->size());
            bindings.resize(aargs->size());
            for (Size i=0;i<aargs->size();++i) {
                args[i] = aargs->data[i];
                bindings[i] = Scalar(_null);
            }
            _msg.clear();
            _rmsg.clear();
            auto pos = XLSession::instance().currentPosition();
            _msg.insertChar(char('e'));
            _msg.insert(Integer(pos->id()));
            _msg.insert(Integer(pos->row()));
            _msg.insert(Integer(pos->col()));
            _msg.insert(transpose);
            _msg.insert(scalars->data);
            _msg.insert(args);
            _msg.insert(bindings);
           _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                std::shared_ptr< Range<Scalar> > range( new Range<Scalar>(1, 1));
                ScalaSession::instance().cout(error);
                range->data[0] = Scalar(_null);
                return range;
            } else {
                auto res = _rmsg.getScalarVector();
                std::shared_ptr< Range<Scalar> > range(new Range<Scalar>(res.size(), 1));
                std::copy(res.begin(), res.end(), range->data.begin());
                return range;
            }
        }

        inline Scalar attr(const Scalar& scalar, const std::string& attr,
                               const std::shared_ptr< Range<Scalar> >& aargs) const {
            std::vector<Scalar> args;
            std::vector<Scalar> bindings;
            args.resize(aargs->size());
            bindings.resize(aargs->size());
            for (Size i=0;i<aargs->size();++i) {
                args[i] = aargs->data[i];
                bindings[i] = Scalar(_null);
            }
            _msg.clear();
            _rmsg.clear();
            auto pos = XLSession::instance().currentPosition();
            _msg.insertChar(char('a'));
            _msg.insert(Integer(pos->id()));
            _msg.insert(Integer(pos->row()));
            _msg.insert(Integer(pos->col()));
            _msg.insert(false);
            _msg.insert(scalar);
            _msg.insert(attr);
            _msg.insert(args);
            _msg.insert(bindings);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                ScalaSession::instance().cout(error);
                return Scalar(_null);
            } else {
                return _rmsg.getScalar();
            }
        }

        inline std::shared_ptr< Range<Scalar> > get(const Scalar& scalar,
                                                    const std::shared_ptr< Range<Scalar> >& aargs,
                                                    bool transpose,
                                                    bool force) const {
            std::vector<Scalar> args;
            std::vector<Scalar> bindings;
            args.resize(aargs->size());
            bindings.resize(aargs->size());
            for (Size i=0;i<aargs->size();++i) {
                args[i] = aargs->data[i];
                bindings[i] = Scalar(_null);
            }
             _msg.clear();
            _rmsg.clear();
            auto pos = XLSession::instance().currentPosition();
            _msg.insertChar(char('g'));
            _msg.insert(Integer(pos->id()));
            _msg.insert(Integer(pos->row()));
            _msg.insert(Integer(pos->col()));
            _msg.insert(transpose);
            _msg.insert(scalar);
            _msg.insert(args);
            _msg.insert(bindings);
            _msg.insert(force);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                std::shared_ptr< Range<Scalar> > range( new Range<Scalar>(1, 1));
                ScalaSession::instance().cout(error);
                range->data[0] = Scalar(_null);
                return range;
            } else {
                auto res1 = _rmsg.getScalarVector();
                auto res2 = _rmsg.getScalarVector();
                if (res2.size() == 1) {
                    std::shared_ptr< Range<Scalar> > range(new Range<Scalar>(res1.size(), 1));
                    std::copy(res1.begin(), res1.end(), range->data.begin());
                    return range;
                } else if (res2.size() == 2) {
                    int size1 = res2[0].get<int>();
                    int size2 = res2[1].get<int>();
                    std::shared_ptr< Range<Scalar> > range(new Range<Scalar>(size1, size2));
                    std::copy(res1.begin(), res1.end(), range->data.begin());
                } else {
                    FAIL ("unsuported size for get return");
                }
            }
        }

        inline std::shared_ptr< Range<Scalar> > getAttr(const Scalar& scalar, const std::string& attr,
                                                        const std::shared_ptr< Range<Scalar> >& aargs,
                                                        bool transpose,
                                                        bool force) const {
            std::vector<Scalar> args;
            std::vector<Scalar> bindings;
            args.resize(aargs->size());
            bindings.resize(aargs->size());
            for (Size i=0;i<aargs->size();++i) {
                args[i] = aargs->data[i];
                bindings[i] = Scalar(_null);
            }
            _msg.clear();
            _rmsg.clear();
            auto pos = XLSession::instance().currentPosition();
            _msg.insertChar(char('b'));
            _msg.insert(Integer(pos->id()));
            _msg.insert(Integer(pos->row()));
            _msg.insert(Integer(pos->col()));
            _msg.insert(transpose);
            _msg.insert(scalar);
            _msg.insert(attr);
            _msg.insert(args);
            _msg.insert(bindings);
            _msg.insert(force);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                std::shared_ptr< Range<Scalar> > range( new Range<Scalar>(1, 1));
                ScalaSession::instance().cout(error);
                range->data[0] = Scalar(_null);
                return range;
            } else {
                auto res1 = _rmsg.getScalarVector();
                auto res2 = _rmsg.getScalarVector();
                if (res2.size() == 1) {
                    std::shared_ptr< Range<Scalar> > range(new Range<Scalar>(res1.size(), 1));
                    std::copy(res1.begin(), res1.end(), range->data.begin());
                    return range;
                } else if (res2.size() == 2) {
                    int size1 = res2[0].get<int>();
                    int size2 = res2[1].get<int>();
                    std::shared_ptr< Range<Scalar> > range(new Range<Scalar>(size1, size2));
                    std::copy(res1.begin(), res1.end(), range->data.begin());
                } else {
                    FAIL ("unsuported size for getAttr return");
                }
            }
        }

        inline Scalar getType(const Scalar& scalar) const {
            _msg.clear();
            _rmsg.clear();
            _msg.insertChar(char('t'));
            _msg.insert(scalar);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                ScalaSession::instance().cout(error);
                return Scalar(error);
            } else {
                return _rmsg.getScalar();
            }
        }

        inline std::shared_ptr< Range<Scalar> > listAttr(const Scalar& scalar) const {
            _msg.clear();
            _rmsg.clear();
            _msg.insertChar(char('l'));
            _msg.insert(scalar);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                std::shared_ptr< Range<Scalar> > range( new Range<Scalar>(1, 1));
                ScalaSession::instance().cout(error);
                range->data[0] = Scalar(_null);
                return range;
            } else {
                auto res = _rmsg.getScalarVector();
                std::shared_ptr< Range<Scalar> > range(new Range<Scalar>(res.size(), 1));
                std::copy(res.begin(), res.end(), range->data.begin());
                return range;
            }
        }

        inline Scalar use(const std::shared_ptr< Range<Scalar> >& scalars) const {
            _msg.clear();
            _rmsg.clear();
            _msg.insertChar(char('u'));
            _msg.insert(scalars->data);
            _socket(_msg, _rmsg);
            std::string error = _rmsg.getError();
            if (error.size()) {
                ScalaSession::instance().cout(error);
                return Scalar(_null);
            } else {
                return _rmsg.getScalar();
            }
        }

        inline void connect() {
            _socket.tryconnect();
        }

    protected:
        SocketClient<XLSCALA_BUFSIZE> _socket;
        Null _null;
        mutable boost::circular_buffer<std::string> _cout;
        mutable MessagePacket<XLSCALA_BUFSIZE> _msg;
        mutable MessagePacket<XLSCALA_BUFSIZE> _rmsg;
	};

}

#endif

