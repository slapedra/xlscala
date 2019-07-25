package xlscala
   
import scala.collection.mutable.ArrayBuffer
 
import javax.swing._
import java.awt._
import java.awt.event._

import xlscala.interpreter._
import xlscala.net._
 

import java.io._
import java.net._
import java.awt._

import scala.collection.mutable.ArrayBuffer
import scala.collection.immutable.List
 
object XLScalaServer { 
	
	protected class SplashScreen(val url :String) extends JWindow {		
		var content = getContentPane.asInstanceOf[JPanel]
		content.setBackground(Color.white)
		var screen = Toolkit.getDefaultToolkit.getScreenSize
		var imageURL = getClass.getResource(url)
		require(imageURL != null, "error loading icon: " + url)
		var _icon = Toolkit.getDefaultToolkit.getImage(imageURL)
		var mediaTracker = new MediaTracker(this)
		mediaTracker.addImage (_icon, 0)
		mediaTracker.waitForID(0)
		var _width = _icon.getWidth(null)
		var _height = _icon.getHeight(null)
		var _x = (screen.width-_width)/2
		var _y = (screen.height-_height)/2
		setBounds(_x, _y, _width, _height)
		
		override def paint (g :Graphics) {
			g.drawImage (_icon, 0, 0, _width, _height, this);
		}
    }
	
	protected var _splash = new SplashScreen("data/xlscala.png")
	_splash.setVisible(true)	
	
	protected val _version = "1.0.0"
	protected val _interpreter = new ScalaInterpreter()
	
	protected val _sessions :ArrayBuffer[InterpreterSession] = ArrayBuffer()
	   
	final class InterpreterSession extends Function2[MessagePacket, MessagePacket, Boolean] {
		 
		protected var _context = new ScalaInterpreterContext
		protected var _namorMap = new ExcelNamorMap
	 
		def clear = _context.clear
		
		@inline
		def apply(msg :MessagePacket, rmsg :MessagePacket) :Boolean = {
			try {
				var f = msg.toAny
				f match {
					case 's' => {
						var sheet = msg.toAny.asInstanceOf[Int]
						var row = msg.toAny.asInstanceOf[Int]
						var col = msg.toAny.asInstanceOf[Int]
						var transpose = msg.toAny.asInstanceOf[Boolean]
						var scalar = msg.toAny
						var args = msg.toAny.asInstanceOf[Array[Any]]
						var bindings = msg.toAny.asInstanceOf[Array[Any]]
						var values = msg.toAny.asInstanceOf[Array[Any]]
						var indexes = msg.toAny.asInstanceOf[Array[Any]]
						var namor = new ExcelNamor(_namorMap, sheet, row, col, transpose) 
						rmsg += _interpreter.set(_context, namor, scalar, args, bindings, values, indexes)
					}
					case 'e' => {
						var sheet = msg.toAny.asInstanceOf[Int]
						var row = msg.toAny.asInstanceOf[Int]
						var col = msg.toAny.asInstanceOf[Int]
						var transpose = msg.toAny.asInstanceOf[Boolean]
						var scalars = msg.toAny.asInstanceOf[Array[Any]]
						var args = msg.toAny.asInstanceOf[Array[Any]]
						var bindings = msg.toAny.asInstanceOf[Array[Any]]
						var namor = new ExcelNamor(_namorMap, sheet, row, col, transpose)
						var codes :ArrayBuffer[String] = ArrayBuffer()
						for (scalar <- scalars) {
							codes += scalar.asInstanceOf[String]
						}
						rmsg ++= _interpreter.exec(_context, namor, codes, args, bindings)
					}
					case 'a' => {
						var sheet = msg.toAny.asInstanceOf[Int]
						var row = msg.toAny.asInstanceOf[Int]
						var col = msg.toAny.asInstanceOf[Int]
						var transpose = msg.toAny.asInstanceOf[Boolean]
						var scalar = msg.toAny
						var attr = msg.toAny.asInstanceOf[String]
						var args = msg.toAny.asInstanceOf[Array[Any]]
						var bindings = msg.toAny.asInstanceOf[Array[Any]]
						var namor = new ExcelNamor(_namorMap, sheet, row, col, transpose) 
						rmsg += _interpreter.attr(_context, namor, scalar, attr, args, bindings)
					}
					case 'g' => {
						var sheet = msg.toAny.asInstanceOf[Int]
						var row = msg.toAny.asInstanceOf[Int]
		 				var col = msg.toAny.asInstanceOf[Int]
						var transpose = msg.toAny.asInstanceOf[Boolean]
						var scalar = msg.toAny
						var args = msg.toAny.asInstanceOf[Array[Any]]
						var bindings = msg.toAny.asInstanceOf[Array[Any]]
						var force = msg.toAny.asInstanceOf[Boolean]
						var namor = new ExcelNamor(_namorMap, sheet, row, col, transpose) 
						var ret = _interpreter.get(_context, namor, scalar, args, bindings, force)
						rmsg ++= ret._1
						rmsg ++= ret._2
					}
					case 'b' => {
						var sheet = msg.toAny.asInstanceOf[Int]
						var row = msg.toAny.asInstanceOf[Int]
						var col = msg.toAny.asInstanceOf[Int]
						var transpose = msg.toAny.asInstanceOf[Boolean]
						var scalar = msg.toAny
						var attr = msg.toAny.asInstanceOf[String]
						var args = msg.toAny.asInstanceOf[Array[Any]]
						var bindings = msg.toAny.asInstanceOf[Array[Any]]
						var force = msg.toAny.asInstanceOf[Boolean]
						var namor = new ExcelNamor(_namorMap, sheet, row, col, transpose) 
						var ret = _interpreter.getAttr(_context, namor, scalar, attr, args, bindings, force)
						rmsg ++= ret._1
						rmsg ++= ret._2
					}   
					case 't' => {
						var scalar = msg.toAny
						rmsg += _interpreter.getType(_context, scalar)
					} 
					case 'l' => {
						var scalar = msg.toAny
						rmsg += _interpreter.listAttr(_context, scalar)
					} 
					case 'u' => {
						var scalars = msg.toAny.asInstanceOf[Array[Any]]
						var iter = scalars.iterator
						var imports :Array[String] = Array.fill(scalars.size){iter.next.asInstanceOf[String]}
						_interpreter.use(_context, imports)
						rmsg += true
					} 
					case e :Throwable => {
						rmsg.putError(e.toString)
					}
				}
				return false
			} catch { 
				case e :Throwable => {
					rmsg.putError(e.toString)
					return false
				}
			}
		}
	}
	
	def run {
		
		require(SystemTray.isSupported, "system tray not supported")
		var iniStream = new java.io.FileInputStream(".\\xlscala.ini")
		require(iniStream != null, "error loading ini file")
		var in = new java.io.BufferedReader(new java.io.InputStreamReader(iniStream))
		in.readLine
		var port = in.readLine.toInt
		var server = SocketServer(port)
		port = server.port	
		var host = java.net.InetAddress.getLocalHost.getHostAddress
		var popup = new PopupMenu();
		var imageURL = getClass.getResource("data/xlscala.png")
		require(imageURL != null, "error loading icon")
		var icon = Toolkit.getDefaultToolkit().getImage(imageURL)
		var trayIcon = new TrayIcon(icon, "XLScala", popup)
		trayIcon.setToolTip("XLScala")
		trayIcon.setImageAutoSize(true)
		var tray = SystemTray.getSystemTray
		var fontb = new Font("Times New Roman", Font.BOLD, 11)
		var font = new Font("Times New Roman", Font.PLAIN, 11)
		var mItem1 = new MenuItem("XLScala")
		mItem1.setFont(fontb)
		popup.add(mItem1)
		popup.addSeparator
		var mItem2 = new MenuItem("Version: " + _version)
		mItem2.setEnabled(false)
		mItem2.setFont(font)
		popup.add(mItem2)
		var mItem3 = new MenuItem("Author: Sebastien Lapedra")
		mItem3.setEnabled(false)
		mItem3.setFont(font)
		popup.add(mItem3)
		var mItem4 = new MenuItem("Host: " + host)
		mItem4.setEnabled(false)
		mItem4.setFont(font)
		popup.add(mItem4)
		var mItem5 = new MenuItem("Port: " + port)
		mItem5.setEnabled(false)
		mItem5.setFont(font)
		popup.add(mItem5)
		popup.addSeparator
		var mItem7 = new MenuItem("Clear");
		mItem7.setFont(font)
		mItem7.addActionListener(new ActionListener() {
			def actionPerformed(e :ActionEvent) {
				_interpreter.clear
				for (session <- _sessions) session.clear
			}
		})
		popup.add(mItem7)
		
		object ShutdownInterceptor extends Thread {
			override def run {
				tray.remove(trayIcon)
				Runtime.getRuntime().removeShutdownHook(ShutdownInterceptor)
				System.exit(0)
			}
		}
		
		var mItem8 = new MenuItem("Exit");
		mItem8.setFont(font)
		mItem8.addActionListener(new ActionListener() {
			def actionPerformed(e :ActionEvent) {
				tray.remove(trayIcon)
				Runtime.getRuntime().removeShutdownHook(ShutdownInterceptor)
				server.close
				System.exit(0)
			}
		});
		popup.add(mItem8)
		var outStream = new java.io.FileOutputStream(".\\xlscala.ini")
		require(outStream != null, "error writing ini file")
		var out = new java.io.OutputStreamWriter(outStream)
		out.write(host)
		out.write(System.getProperty("line.separator"))
		out.write(port.toString)
		out.write(System.getProperty("line.separator"))
		out.close		
		Runtime.getRuntime().addShutdownHook(ShutdownInterceptor)

		try {
			tray.add(trayIcon)
		} catch {
			case _ :Throwable => { 
				System.err.println("Problem loading Tray icon")
				System.exit(0)
			}
		}
		
		trayIcon.displayMessage("XLScala server launched", 
           "Your XLScala server is running",
           TrayIcon.MessageType.INFO);
		
		_splash.setVisible(false)		
		
		try {
			while(true) {
				var int = new InterpreterSession
				server.connect(int)
				_sessions += int
			}
		} catch {
			case e :Throwable => {
				trayIcon.displayMessage("XLScala error", 
					e.toString, TrayIcon.MessageType.ERROR)
				Thread.sleep(15000)
				tray.remove(trayIcon)
				Runtime.getRuntime().removeShutdownHook(ShutdownInterceptor)
				server.close
				System.exit(0)
			}
		} 
	}
	
	def main(args :Array[String]) {
		run
	}
	
}


import xlscala.net._

object Test {
	  
	def main(args :Array[String]) {
		var begin = System.currentTimeMillis();
		var client = SocketClient(12345, "localhost")
		var msg = new MessagePacket
		var rmsg = new MessagePacket
		for (x <- 0 until 1) {
			msg.clear
			rmsg.clear
			msg += 's'
			msg += 1
			msg += 2
			msg += 2
			msg += false
			msg += "__1+_2*3"
			msg ++= List(8.8, 10.5)
			msg ++= List(null, null)
			msg ++= List()
			msg ++= List()
		
			client(msg, rmsg)
			println(rmsg.toAny)
			msg.clear
			rmsg.clear
			msg += 'u'
			msg ++= List("chronos._")
			client(msg, rmsg)
			println(rmsg.getError)
			
			/*msg.clear
			rmsg.clear
			msg += 's'
			msg += 1
			msg += 2
			msg += 2
			msg += false
			msg += "_1 + 2"
			msg ++= List(8.8, 10.5)
			msg ++= List(null, null)
			msg ++= List()
			msg ++= List()
			client(msg, rmsg)
			var obj = rmsg.toAny
			println(obj)
			
			
			msg.clear
			rmsg.clear
			msg += 's'
			msg += 1
			msg += 2
			msg += 2
			msg += false
			msg += "__2"
			msg ++= List(obj, 10.5)
			msg ++= List("1", "2")
			msg ++= List()
			msg ++= List()
			client(msg, rmsg)
			println(rmsg.toAny)*/
		}
		var end = System.currentTimeMillis();
		println("\t\t ... done in " + (end - begin) / 1000.0 + " seconds")
		client.close
	}
}
