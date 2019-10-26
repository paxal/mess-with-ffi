<?php

declare(strict_types=1);

namespace Paxal\FFITest;

use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;

final class RandomValueCommand extends Command
{
    protected function configure()
    {
        $this
            ->setName('random')
            ->addOption('min', null, InputOption::VALUE_REQUIRED, 'Min', 0)
            ->addOption('max', null, InputOption::VALUE_REQUIRED, 'Max', 1000);
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $min = (int) $input->getOption('min');
        $max = (int) $input->getOption('max');

        $output->write(sprintf('Random integer between %d and %d : ', $min, $max));
        $output->writeln((string) random_int($min, $max));

        return 0;
    }
}
