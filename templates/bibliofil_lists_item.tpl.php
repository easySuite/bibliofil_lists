<?php

/**
 * @file
 * Bibliofil Lists item template.
 *
 * @var string $opac_url
 * @var array $data
 */
?>
<a href="<?php print $opac_url . $data['id']; ?>">
  <img src="<?php print $data['cover_src']; ?>" alt="<?php print $data['title']; ?>">
</a>
