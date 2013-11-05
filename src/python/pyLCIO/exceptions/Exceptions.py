class LcioException(Exception):
    def __init__( self, message ):
        super(LcioException, self).__init__(message)
        self.type = 'lcio::Exception'
    def __str__( self ):
        return repr(self.type + ': ' + self.message)

class EventException(LcioException):
    def __init__( self, message ):
        super(EventException, self).__init__(message)
        self.type = 'lcio::EventException'
        
class DataNotAvailableException(EventException):
    def __init__( self, message ):
        super(DataNotAvailableException, self).__init__(message)
        self.type = 'lcio::DataNotAvailableException'
        
class ReadOnlyException(EventException):
    def __init__( self, message ):
        super(ReadOnlyException, self).__init__(message)
        self.type = 'lcio::ReadOnlyException'

class IOException(LcioException):
    def __init__( self, message ):
        super(IOException, self).__init__(message)
        self.type = 'lcio::IOException'
        
class EndOfDataException(IOException):
    def __init__( self, message ):
        super(EndOfDataException, self).__init__(message)
        self.type = 'lcio::EndOfDataException'
