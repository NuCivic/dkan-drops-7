module.exports = function(grunt) {

  grunt.initConfig({

    pkg: grunt.file.readJSON('package.json'),

    clean: {
      js: ['dist'],
    },

    watch: {
      js: {
        files: 'src/**/*.js',
        tasks: ['concat:development']
      },
    },

    uglify: {
      production: {
        files: [
          {
            expand: true,
            cwd: 'dist',
            src: ['**/*.js', '!**/*.min.js'],
            dest: 'dist',
            ext: '.min.js'
          }
        ]
      }
    },

    concat: {
      always: {
        files: [
          {
            expand: true,
            cwd: 'src',
            src: '**/*.js',
            dest: 'dist',
          }
        ]
      },
      development: {
        files: ['<%= uglify.production.files %>']
      }
    },

    jshint: {
      options: {
        browser: true,
        devel: true,
        freeze: true,
        jquery: true,
        node: true,
        undef: true,
        unused: true,
        globals: {
          jQuery: true,
          Drupal: true,
          Western: true
        }
      },
      development: {
        files: {
          src: ['Gruntfile.js', 'src/**/*.js']
        },
        options: {
          force: true
        }
      }
    },

  });

  grunt.loadNpmTasks('grunt-contrib-clean');
  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-newer');

  grunt.registerTask(
    'default',
    'The development task suite.',
    [
      'concat:always',
      'concat:development',
      'jshint:development'
    ]
  );

  grunt.registerTask(
    'build',
    'The production build task suite.',
    [
      'concat:always',
      'uglify:production',
    ]
  );

};
