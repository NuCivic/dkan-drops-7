# Autoload

Want to not care about loading classes, traits or interfaces? The aim of this project is what you are looking for!

## How it works?

The module scans all Drupal infrastructure, generates a class map with all possible namespaces and stores it to `DRUPAL_ROOT . '/autoload.php'` (path can be changed by modifying the `autoload_file` Drupal variable). If a file is not writable (which must not be the case during development) then a class map will be dumped into the database.

Note, that after downloading projects via Drush (e.g. `drush dl ctools_api pinmap`) the autoloading class map will be automatically rebuilt (only once, no matter how many projects were downloaded).

### How to correctly add new classes/traits/interfaces?

Well, since the autoloading - it is a generated static file, you need to update it once you adding new classes, traits or interfaces. Ideally, after removal as well in order to not keep outdated entries inside of the map thereby prevent its expansion.

**In short**: added a new file? Execute `drush aur` and continue working.

### How to correctly remove a module?

First of all, it's applies only to modules, which use autoloading. If you decided to remove the project from filesystem then after the action execute the `drush autoload-rebuild` or `drush aur` to rebuild the autoloading class map.

## Usage

You able to use one or both of known autoloading standards - [PSR-0](http://www.php-fig.org/psr/psr-0) and [PSR-4](http://www.php-fig.org/psr/psr-4).

Some of the projects using autoloading:

- [CTools API](https://www.drupal.org/project/ctools_api)
- [Commerce Utilities](https://www.drupal.org/project/commerce_utils)
- [Commerce Adyen](https://www.drupal.org/project/commerce_adyen)
- [Commerce Bangkok Bank iPay](https://www.drupal.org/project/commerce_bangkokbank)
- [Commerce PayGate PayHost](https://www.drupal.org/project/commerce_paygate_payhost)

### Drupal way

Add `autolaod = TRUE` inside of module's `*.info` file and just place the objects inside of `lib/Drupal/YOUR_MODULE_NAME` and/or `src` directories.

```ini
; All namespaces must be followed by "Drupal\YOUR_MODULE_NAME".
autoload = TRUE
```

Have a look at [tests](tests/autoload_test_drupal) as an example.

### Custom namespaces

**Please, avoid usage of custom namespaces due to their non-standardisation. They can cause a pain in the ass and supports only for backward compatibility.**

Configure the `autoload` directive as an array where keys are subdirectories inside of the module directory and values - object namespaces. As many as needed directories and namespaces can be added in this way.

#### PSR-0

```ini
; All objects, namespace path of which starts from "CTools",
; will be searched inside of "<MODULE_PATH>/psr-0".
autoload[psr-0][] = CTools
```

#### PSR-4

```ini
; All objects, namespace path of which starts from "CTools\Plugins",
; will be searched inside of "<MODULE_PATH>/psr-4".
autoload[psr-4][] = CTools\Plugins
```

#### PSR-4 (single namespace)

Take into account the trailing slash! It must be at the end of global namespace to use `PSR-4` standard. It looks similar to `PSR-0`, but that slash telling that it's not true.

```ini
; All objects, namespace path of which starts from "CTools",
; will be searched inside of "<MODULE_PATH>/psr-4".
autoload[psr-4][] = CTools\
```

Have a look at [tests](tests/autoload_test_custom) as an example.
