<?php

declare(strict_types=1);

namespace Paxal\FFITest;

use Paxal\FFITest\FinalClasses\ExtendsFinalClass;
use Paxal\FFITest\FinalClasses\FinalClass;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;

final class UnfinalCommand extends Command
{
    protected function configure(): void
    {
        $this
            ->setName('no-final')
            ->setDescription('Will load FinalClass class which is final and make it not final');
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        ffi_no_final(FinalClass::class);
        $object = new ExtendsFinalClass();
        $output->writeln(
            sprintf(
                'object of class %s extends %s : %s',
                get_class($object),
                FinalClass::class,
                @json_encode($object instanceof FinalClass)
            )
        );

        return 0;
    }
}
