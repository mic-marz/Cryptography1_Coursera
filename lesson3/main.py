#! /usr/bin/python

import sys,os
from Crypto.Hash import SHA256

blockLen=1024


class ChunkIterator:
    def __init__(self,inputFile,inputFileSize):
        self.refFile=inputFile
        self.fileSize=inputFileSize
        self.rest=self.fileSize%blockLen
        self.pointer=self.rest
        self.numBytes=self.rest


    def __iter__(self):
        return self

    def next(self):
        if self.pointer>self.fileSize:
            raise StopIteration
        
        self.refFile.seek(-self.pointer, os.SEEK_END)
        chunk=self.refFile.read(self.numBytes) 

        self.pointer+=blockLen
        self.numBytes=blockLen

        return chunk
        

class FileWrapper:
    def __init__(self,inputFile=None):
        try:
            self.f=open(inputFile, 'rb')
            self.statInfo=os.stat(inputFile)
            self.fileSize=self.statInfo.st_size
            self.chunkIterator=ChunkIterator(self.f, self.fileSize)
        except IOError as e:
            print 'Error in reading file ' + inputFile
            raise

#Iterator protocol implementation
    def __iter__(self):
        return self.chunkIterator


#Context manager protocol implementation
    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.f.close()
        return False    ## Returning true, exception is suppressed on the caller
        

def calculateHash(inputFile):
   try: 
        with FileWrapper(inputFile) as fileHandle:
            appendedTag=''
            hashTempObject=SHA256.new()

# The StopIteration exception raised by iterator (non terminating one) is catched 
# in the for loop by Python to terminate it!
            for fileChunk in fileHandle:    
                composedChunk=fileChunk + appendedTag
                hashTempObject=SHA256.new()
                hashTempObject.update(composedChunk)
                appendedTag=hashTempObject.digest()

   except BaseException as e:
        print 'Problems in getting final digest!' 
        print e.__str__()
   
   else:
        finalTag=hashTempObject.hexdigest()
        print 'The final digest is: {0}'.format(finalTag)



def chunkGenerator(inputFile):
    try:
        with open(inputFile, 'rb') as f:
            statInfo=os.stat(inputFile)
            fileSize=statInfo.st_size

            rest=fileSize%blockLen
            pointer=rest
            numBytes=rest

            while True:
                if pointer>fileSize:    
                    raise StopIteration
            
                f.seek(-pointer, os.SEEK_END)
                chunk=f.read(numBytes) 
                
                yield chunk

                pointer+=blockLen
                numBytes=blockLen

    except IOError as e:
        print 'Error in reading file ' + inputFile
        raise
        

def calculateHashWithGenerator(inputFile):
    generator=chunkGenerator(inputFile)
    appendedTag=''
    hashTempObject=SHA256.new()

    try: 
        while True:                      # Obviously it's possibile to use for loop without StopIteration except specification
            fileChunk=generator.next()   # The first execution of generator function begins with the first invocation of next() iterator function
            composedChunk=fileChunk + appendedTag
            hashTempObject=SHA256.new()
            hashTempObject.update(composedChunk)
            appendedTag=hashTempObject.digest()

    except StopIteration as e:
        finalTag=hashTempObject.hexdigest()
        print 'The final digest is: {0}'.format(finalTag)
    except IOError as e:
        print 'Problems in getting final digest!'


class InputError:
    def __init__(self, value):
        self.wrongValue=value
        
    def __str__(self):
        print '{0} value passed to protocol option is wrong!'.format(self.wrongValue)


if __name__=='__main__':
    try: 
        if sys.argv[1]=='-1':
            #Use of basic iterator protocol
            calculateHash(sys.argv[2])
        elif sys.argv[1]=='-2':
            #Use of iterator by means of generator function
            calculateHashWithGenerator(sys.argv[2])   
        else: 
            raise InputError, sys.argv[1]
    except (IndexError, InputError) as e:
        print 'Error in launching hash calculation program!'
        print 'Usage: {0} -p [-1 | -2 ] <input file>'.format(sys.argv[0])
        print 'where "-1" refers to basic iterator protocol,'
        print 'while "-2" refers to generator function iterator protocol.'

