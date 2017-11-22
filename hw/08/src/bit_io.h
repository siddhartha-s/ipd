#pragma once

/*
 * bit_io.h: classes for reading and writing files one bit (or more)
 * at a time.
 *
 * The main idea is that we create a bit input or output stream as a
 * layer over a file that we open for reading or writing. The library
 * maintains a buffer of bits so that the client can view the file as a
 * simple sequence of bits rather than bytes.
 */

#include <istream>
#include <ostream>
#include <fstream>
#include <vector>

namespace ipd {
    /*
     * INPUT
     */

    // A bit input stream, for reading individual bits from a file or a std::vector<unsigned char>
    class bistream {
    public:
        // Reads a bit from this bit input stream into the given bool
        // reference.
        //
        // Parameters:
        //
        //      result - bool reference to store the bit that was read
        //
        // Returns: the same bit input stream, for method chaining
        //
        // Example:
        //
        //      bool b;
        //      bif.read(b);
        //
        bistream &read(bool &result);

        // Reads `n` bits, interpreted as a big-endian `n`-bit integer,
        // and stores them in the reference `result`, which must have a
        // numeric type.
        //
        // Parameters:
        //
        //      <typename T> - the type of the result; must be numeric
        //      result - reference for storing the result
        //      n - number of bits to read
        //
        // Returns: the same bit input stream, for method chaining
        //
        // Example:
        //
        //      int result;
        //      bif.read_bits(result, 5); // reads 5 bits into result
        //
        template<typename T>
        bistream &read_bits(T &result, size_t n);

        // Determines whether we've reached the end of the input file.
        //
        // Returns: `true` if we’ve attempted to read past the end of
        // the file, and `false` otherwise.
        //
        // Example:
        //
        //      while (bif.read(b) && !bif.eof()) {
        //          ...
        //      }
        //
        virtual bool eof() const = 0;

        // Determines the status of the bit input stream.
        //
        // Returns: `false` if there are no bits to read and a read
        // error has occurred, and `true` otherwise.
        //
        // Example:
        //
        //      bif.read(b);
        //      if (bif.good()) {
        //          // we know the read succeeded
        //      }
        //
        virtual bool good() const = 0;

        // The bit input stream boolean coercion operator, inserted, for
        // example, when a `bifstream` is used as a condition for an
        // `if`. Alias for `good()`.
        //
        // Returns: `false` if there are no bits to read and a read
        // error has occurred, and `true` otherwise.
        //
        // Example:
        //
        //      bif.read(b);
        //      if (bif) {
        //          // we know the read succeeded
        //      }
        //
        operator bool() const;

    protected:

        virtual bool next_byte(char &) = 0;

        short nbits = 0;

    private:

        char bitbuf = 0;

    };

    class bifstream : public bistream {
    public:
        // Constructs a bit input stream to read from the given file.
        //
        // Parameters:
        //
        //      filespec - name of the file to open
        //
        // Example:
        //
        //      bifstream bif(input_file_name);
        //
        explicit bifstream(const char *filespec);

        bool eof() const override;

        bool good() const override;

        bifstream(const bifstream &) = delete;

    private:
        std::ifstream stream;

        virtual bool next_byte(char &) override;

    };

    class bistringstream : public bistream {
    public:
        // Constructs a bit input stream to read from the given file.
        //
        // Parameters:
        //
        //      filespec - name of the file to open
        //
        // Example:
        //
        //      bifstream bif(input_file_name);
        //
        explicit bistringstream(std::vector<unsigned char>);

        bool eof() const override;

        bool good() const override;

        bistringstream(const bistringstream &) = delete;

    private:
        size_t bytes_index;
        std::vector<unsigned char> bytes;
        size_t total_bits;

        virtual bool next_byte(char &) override;

    };

    // The bit input stream extraction operator; alias for `read(bool&)`.
    //
    // Params:
    //
    //     - bif - the bit input stream to read from
    //     - b - reference to store the bit that was read
    //
    // Returns: the same bit input stream, for method chaining
    //
    // Example:
    //
    //     bool b1, b2, b3;
    //     bif >> b1 >> b2 >> b3;
    //
    bistream &operator>>(bistream &bif, bool &b);

    /*
     * OUTPUT
     */

    // A bit output stream, for writing individual bits to a file.
    class bostream {
    public:
        // Writes a bit to this bit output stream.
        //
        // Parameters:
        //
        //      b - the bit to write
        //
        // Returns: the same bit output stream, for method chaining
        //
        // Example:
        //
        //      bof.write(true);
        //
        virtual bostream &write(bool b) = 0;

        // Writes an `n`-bit big-endian representation of `value`, which
        // must have a numeric type, to this bit output sream.
        //
        // Parameters:
        //
        //      <typename T> - the type of the value to write; must be numeric
        //      value - the value to write
        //      n - number of bits to write
        //
        // Returns: the same bit output stream, for method chaining
        //
        // Example:
        //
        //      bof.write(22, 5); // writes 10110
        //      bof.write(22, 6); // writes 010110
        //
        template<typename T>
        bostream &write_bits(T value, size_t n);

        // Determines the status of the bit output stream.
        //
        // Returns: `false` if an error has occurred; `true` otherwise
        //
        // Example:
        //
        //      bof.write(b);
        //      if (bof.good()) {
        //          // we know the write succeeded
        //      }
        //
        virtual bool good() const = 0;

        // The bit output stream boolean coercion operator, inserted, for
        // example, when a `bofstream` is used as a condition for an
        // `if`. Alias for `good()`.
        //
        // Returns: `false` if an error has occurred; `true` otherwise
        //
        // Example:
        //
        //      bof.write(b);
        //      if (bof) {
        //          // we know the write succeeded
        //      }
        //
        operator bool() const;

    };

    class bofstream : public bostream {
    public:
        // Constructs a bit output stream to write to the given file.
        //
        // Parameters:
        //
        //      filespec - name of the file to open or create
        //
        // Example:
        //
        //      bofstream bof(output_file_name);
        //
        explicit bofstream(const char *filespec);

        virtual bofstream &write(bool b) override;

        virtual bool good() const override;

        ~bofstream();

        bofstream(const bofstream &) = delete;

    private:
        char bitbuf;
        short nbits;
        std::ofstream stream;

        void write_out();
    };

    class bostringstream : public bostream {
    public:
        std::vector<unsigned char> get_data();

        virtual bostringstream &write(bool b) override;

        virtual bool good() const override;

        size_t bits_written() const;

    private:
        size_t bits_written_ = 0;
        std::vector<unsigned char> data_;

    };

    // The bit output stream insertion operator; alias for
    // `write(bool)`.
    //
    // Params:
    //
    //     - bof - the bit input stream to write to
    //     - b - the bit to write
    //
    // Returns: the same bit output stream, for method chaining
    //
    // Example:
    //
    //     bof << true << false << false;
    //
    bofstream &operator<<(bofstream &bof, bool b);

    /*
     * TEMPLATE IMPLEMENTATIONS
     */

    template<typename T>
    bistream &bistream::read_bits(T &result, size_t n) {
        bool bit;

        result = 0;

        while (n--) {
            read(bit);
            result = result << 1 | bit;
        }

        return *this;
    }

    inline bistream &operator>>(bistream &bif, bool &bit) {
        return bif.read(bit);
    }

    template<typename T>
    bostream &bostream::write_bits(T value, size_t n) {
        while (n--) {
            write(value >> n & 1);
        }

        return *this;
    }

    inline bostream &operator<<(bostream &bof, bool bit) {
        return bof.write(bit);
    }
}
